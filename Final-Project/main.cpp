#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/model.h"
#include "include/graph.h"
#include "include/program.h"
#include "include/flowline.h"
#include "include/variables.h"

#include "andres/graph/complete-graph.hxx"
#include "andres/graph/multicut/greedy-additive.hxx"
#include "graph/include/andres/graph/lifting.hxx"
#include "graph/src/command-line-tools/utils.hxx"

template<typename N, typename E>
void checkLine(typename model<N,E>::Line* tLine, FlowLine& tFlow)
{
	if( tLine->isInFlowline )
		return;
	tFlow.lines.push_back( tLine->index );
	bool isEndVertex = true;
	tLine->isInFlowline = true;
	tLine->flowLineID = initialFlowLines.size();
	vector<int>::iterator it;
	for( auto neighbor : tLine->vertices[0]->lines )
	{
		if( neighbor->clusterID == tLine->clusterID && neighbor->index != tLine->index )
			isEndVertex = false;
		if( neighbor->clusterID != tLine->clusterID || neighbor->index == tLine->index || neighbor->isInFlowline )
			continue;
		//auto node = neighbor;
		tFlow.vertices.push_back( tLine->vertices[0]->index );
		tLine->vertices[0]->crossedFlowLines.push_back( tLine->flowLineID );
		checkLine<float, float>(neighbor, tFlow);
		break;
	}
	if( isEndVertex )
	{
		tFlow.endVertices.push_back( tLine->vertices[0]->index );
		//tFlow.vertices.erase( find( tFlow.vertices.begin(), tFlow.vertices.end(), tLine->vertices[0]->index ) );
		tLine->vertices[0]->flowLines.push_back( tLine->flowLineID );
		/*tLine->vertices[0]->crossedFlowLines.erase( find( tLine->vertices[0]->crossedFlowLines.begin(),
														  tLine->vertices[0]->crossedFlowLines.end(),
														  tLine->flowLineID ) );*/
	}

	isEndVertex = true;

	for( auto neighbor : tLine->vertices[1]->lines )
	{
		if( neighbor->clusterID == tLine->clusterID && neighbor->index != tLine->index )
			isEndVertex = false;
		if( neighbor->clusterID != tLine->clusterID || neighbor->index == tLine->index || neighbor->isInFlowline )
			continue;
		//auto node = neighbor;
		tFlow.vertices.push_back( tLine->vertices[1]->index );
		tLine->vertices[1]->crossedFlowLines.push_back( tLine->flowLineID );
		checkLine<float, float>(neighbor, tFlow);
		break;
	}
	if( isEndVertex )
	{
		tFlow.endVertices.push_back( tLine->vertices[1]->index );
		//tFlow.vertices.erase( find( tFlow.vertices.begin(), tFlow.vertices.end(), tLine->vertices[1]->index ) );
		tLine->vertices[1]->flowLines.push_back( tLine->flowLineID );
		/*tLine->vertices[1]->crossedFlowLines.erase( find( tLine->vertices[1]->crossedFlowLines.begin(),
														  tLine->vertices[1]->crossedFlowLines.end(),
														  tLine->flowLineID ) );*/
	}
}

template<typename N, typename E>
void initialFlowLineExtraction(model<N,E> mod)
{
	bool searching = true;
	for( auto& tLine : obj.lines )
	{
		FlowLine tFlow;
		if( tLine->isInFlowline )
			continue;

		checkLine<float, float>(tLine, tFlow);
		tFlow.index = initialFlowLines.size();
		initialFlowLines.push_back( tFlow );
	}

	nFlowLines = initialFlowLines.size();
	float l;
	for( auto& tFlowLine : initialFlowLines )
	{
		l = 0;
		for( int i = 0; i < tFlowLine.lines.size(); i++ )
		{
			auto tLine = obj.lines[ tFlowLine.lines[i] ];
			l += glm::length2( tLine->vertices[0]->pos - tLine->vertices[1]->pos );
		}
		tFlowLine.length = l;
	}
}

void genReliableStrands( vector<FlowLine> &FlowLines )
{
	andres::graph::Graph<> flowLineGraph;
	vector<float> weights;
	flowLineGraph.insertVertices( FlowLines.size() );
	unordered_map< size_t, float > connectedFlowLines;
	set< int > connectedVertices;
	float sigma1 = 7.5, sigma2 = 15, wn = -5;
	for( auto& tFlowLine : FlowLines )
	{
		for( int i = 0; i < tFlowLine.endVertices.size(); i++)
		{
			auto& endVertex = obj.vertices[ tFlowLine.endVertices[i] ];
			for( int indexNeighbor : endVertex->flowLines )
			{
				auto& neighbor = FlowLines[ indexNeighbor ];
				if( connectedVertices.find( endVertex->index ) == connectedVertices.end() &&
					connectedFlowLines.find( key( min( tFlowLine.index, indexNeighbor ), max( tFlowLine.index, indexNeighbor ) ) ) == connectedFlowLines.end() )
					continue;


				if( connectedVertices.find( endVertex->index ) == connectedVertices.end() )
				{
					int counter = 0;
					glm::vec3 avg = glm::vec3( 0.0f, 0.0f, 0.0f ), projectionStart, projectionEnd;
					float dot, det, angle, arg;
					auto vert = endVertex;
					while(counter < 5)
					{
						for( auto tLine : vert->lines )
						{
							if( tLine->flowLineID != tFlowLine.index )
								continue;
							avg += vert->pos - tLine->vertices[ tLine->vertices[0]->index == vert->index ]->pos;
							vert = tLine->vertices[ tLine->vertices[0]->index == vert->index ];
							break;
						}
						counter++;
					}
					avg *= 0.2f;
					projectionStart = avg - glm::dot( avg, endVertex->normal ) / glm::length2( endVertex->normal ) * endVertex->normal;

					counter = 0;
					vert = endVertex;
					avg = glm::vec3( 0.0f, 0.0f, 0.0f );
					while(counter < 5)
					{
						for( auto tLine : vert->lines )
						{
							if( tLine->flowLineID != indexNeighbor )
								continue;
							avg += vert->pos - tLine->vertices[ tLine->vertices[0]->index == vert->index ]->pos;
							vert = tLine->vertices[ tLine->vertices[0]->index == vert->index ];
							break;
						}
						counter++;
					}
					avg *= 0.2f;
					projectionEnd = avg - glm::dot( avg, endVertex->normal ) / glm::length2( endVertex->normal ) * endVertex->normal;

					dot = glm::dot(projectionStart, projectionEnd);
		            det = glm::dot( endVertex->normal, glm::cross( projectionStart, projectionEnd ) );
		            angle = abs( atan2( det, dot ) ) * (180.0/3.141592653589793238463);
					angle = ( angle <= 90 ) ? angle : 180 - angle;
		            arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
		            arg = exp( -arg * arg);
		            arg = ( angle <= 45 ) ? arg : wn * arg;

					connectedFlowLines[ key( min( tFlowLine.index, indexNeighbor ), max( tFlowLine.index, indexNeighbor ) ) ] += arg;
					weights[ flowLineGraph.findEdge( min( tFlowLine.index, indexNeighbor ), max( tFlowLine.index, indexNeighbor ) ).second ] += arg;
				}
				else
				{
					int counter = 0;
					glm::vec3 avg = glm::vec3( 0.0f, 0.0f, 0.0f ), projectionStart, projectionEnd;
					float dot, det, angle, arg;
					auto vert = endVertex;
					while(counter < 5)
					{
						for( auto tLine : vert->lines )
						{
							if( tLine->flowLineID != tFlowLine.index )
								continue;
							avg += vert->pos - tLine->vertices[ tLine->vertices[0]->index == vert->index ]->pos;
							vert = tLine->vertices[ tLine->vertices[0]->index == vert->index ];
							break;
						}
						counter++;
					}
					avg *= 0.2f;
					projectionStart = avg - glm::dot( avg, endVertex->normal ) / glm::length2( endVertex->normal ) * endVertex->normal;

					counter = 0;
					vert = endVertex;
					avg = glm::vec3( 0.0f, 0.0f, 0.0f );
					while(counter < 5)
					{
						for( auto tLine : vert->lines )
						{
							if( tLine->flowLineID != indexNeighbor )
								continue;
							avg += vert->pos - tLine->vertices[ tLine->vertices[0]->index == vert->index ]->pos;
							vert = tLine->vertices[ tLine->vertices[0]->index == vert->index ];
							break;
						}
						counter++;
					}
					avg *= 0.2f;
					projectionEnd = avg - glm::dot( avg, endVertex->normal ) / glm::length2( endVertex->normal ) * endVertex->normal;

					dot = glm::dot(projectionStart, projectionEnd);
		            det = glm::dot( endVertex->normal, glm::cross( projectionStart, projectionEnd ) );
		            angle = abs( atan2( det, dot ) ) * (180.0/3.141592653589793238463);
					angle = ( angle <= 90 ) ? angle : 180 - angle;
		            arg = ( angle <= 45 ) ? ( angle / sigma1 ) : ( ( 90 - angle ) / sigma2 );
		            arg = exp( -arg * arg);
		            arg = ( angle <= 45 ) ? arg : wn * arg;

					flowLineGraph.insertEdge( min( tFlowLine.index, indexNeighbor ), max( tFlowLine.index, indexNeighbor ) );
	                weights.push_back(arg);

					connectedFlowLines.insert( make_pair( key( min( tFlowLine.index, indexNeighbor ), max( tFlowLine.index, indexNeighbor ) ), arg ) );
					connectedVertices.insert( endVertex->index );
				}
			}
		}
	}
	glm::vec3 start, end, vectorStart, vectorEnd;
	float sigma3 = 5, l, dot, det, angle, arg;;
	connectedFlowLines.clear();
	for( Quad tQuad : obj.quads )
    {
		auto tLine1 = obj.insertedLines.find( make_pair( min( tQuad.i1, tQuad.i2 ), max( tQuad.i1, tQuad.i2 ) ) )->second;
		auto tLine2 = obj.insertedLines.find( make_pair( min( tQuad.i3, tQuad.i4 ), max( tQuad.i3, tQuad.i4 ) ) )->second;

		if( tLine1->flowLineID != tLine2->flowLineID )
		{
			start = obj.vertices[ tQuad.i1 ]->pos;
			end = obj.vertices[ tQuad.i2 ]->pos;
			vectorStart = start - end;

			start = obj.vertices[ tQuad.i4 ]->pos;
			end = obj.vertices[ tQuad.i3 ]->pos;
			vectorEnd = start - end;

			dot = glm::dot( glm::normalize( vectorStart ), glm::normalize( vectorEnd ) );
			angle = abs( acos( dot ) ) * (180.0/3.141592653589793238463);
			angle = ( angle <= 90 ) ? angle : 180 - angle;

			arg = angle / sigma3;
			arg = exp( -arg * arg);
			l = min( FlowLines[ tLine1->flowLineID ].length, FlowLines[ tLine2->flowLineID ].length );
			arg = 2 / l * arg;

			if( connectedFlowLines.find( key( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) ) ) == connectedFlowLines.end() )
			{
				connectedFlowLines.insert( make_pair( key( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) ), arg ) );
				flowLineGraph.insertEdge( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) );
				weights.push_back(arg);
			}
			else
				weights[ flowLineGraph.findEdge( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) ).second ] += arg;
		}

		tLine1 = obj.insertedLines.find( make_pair( min( tQuad.i2, tQuad.i3 ), max( tQuad.i2, tQuad.i3 ) ) )->second;
		tLine2 = obj.insertedLines.find( make_pair( min( tQuad.i1, tQuad.i4 ), max( tQuad.i1, tQuad.i4 ) ) )->second;

		if( tLine1->flowLineID != tLine2->flowLineID )
		{
			start = obj.vertices[ tQuad.i2 ]->pos;
			end = obj.vertices[ tQuad.i3 ]->pos;
			vectorStart = start - end;

			start = obj.vertices[ tQuad.i1 ]->pos;
			end = obj.vertices[ tQuad.i4 ]->pos;
			vectorEnd = start - end;

			dot = glm::dot( glm::normalize( vectorStart ), glm::normalize( vectorEnd ) );
			angle = abs( acos( dot ) ) * (180.0/3.141592653589793238463);
			angle = ( angle <= 90 ) ? angle : 180 - angle;

			arg = angle / sigma3;
			arg = exp( -arg * arg);
			l = min( FlowLines[ tLine1->flowLineID ].length, FlowLines[ tLine2->flowLineID ].length );
			arg = 2 / l * arg;

			if( connectedFlowLines.find( key( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) ) ) == connectedFlowLines.end() )
			{
				connectedFlowLines.insert( make_pair( key( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) ), arg ) );
				flowLineGraph.insertEdge( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) );
				weights.push_back(arg);
			}
			else
				weights[ flowLineGraph.findEdge( min( tLine1->flowLineID, tLine2->flowLineID ), max( tLine1->flowLineID, tLine2->flowLineID ) ).second ] += arg;
		}
    }

	connectedFlowLines.clear();
	int fl1, fl2;
	for( auto& tVertex : obj.vertices )
	{
		for( int i = 0; i < tVertex->crossedFlowLines.size(); i++ )
		{
			fl1 = tVertex->crossedFlowLines[i];
			for( int j = i + 1; j < tVertex->crossedFlowLines.size(); j++ )
			{
				fl2 = tVertex->crossedFlowLines[j];
				if( connectedFlowLines.find( key( min( fl1, fl2 ), max( fl1, fl2 ) ) ) != connectedFlowLines.end() )
					continue;

				flowLineGraph.insertEdge( min( fl1, fl2 ), max( fl1, fl2 ) );
				connectedFlowLines.insert( make_pair( key( min( fl1, fl2 ), max( fl1, fl2 ) ), -25 ) );
				weights.push_back( -25 );
			}
		}
	}

	cout<<"\n\nPhase 2\n=======\n";
	cout<<"Correlation clustering of flowlines...\n";
	vector<char> edge_labels( flowLineGraph.numberOfEdges(), 1 );
	andres::graph::multicut::greedyAdditiveEdgeContraction( flowLineGraph, weights, edge_labels );

	auto energy_value = inner_product( weights.begin(), weights.end(), edge_labels.begin(), .0);

    std::vector<size_t> vertex_labels( flowLineGraph.numberOfVertices() );
    edgeToVertexLabels( flowLineGraph, edge_labels, vertex_labels );

	cout<<"Done.\n";
	nClusters = *max_element(vertex_labels.begin(), vertex_labels.end()) + 1;
	cout << "Number of clusters of flowlines: " << nClusters << endl;

}

using namespace glm;

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    glViewport(0,0,width,height);
}

void init (void)
{
	freopen("log.c", "w", stderr);

	//obj.load_model("res/models/spherecylinder/spherecylinder.obj");
	//obj.load_model("res/models/boat-qm/boat-qm.obj");
	obj.load_model("res/models/mug/mug.obj");
    //obj.load_model("res/models/wineglass/wineglass.obj");
	//obj.load_model("res/models/atangana.obj");
	cout<<"readed obj\n";
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glGenBuffers(1, &EBO);

	tGraph.init_graph( obj );
	cout<<obj.indices.size()<<endl;
	cout<<"Starting Correlation clustering...\n";
	vector<char> edge_labels( tGraph.sampleGraph.numberOfEdges(), 1 );
	andres::graph::multicut::greedyAdditiveEdgeContraction( tGraph.sampleGraph, tGraph.weights, edge_labels );

	auto energy_value = inner_product(tGraph.weights.begin(), tGraph.weights.end(), edge_labels.begin(), .0);

    std::vector<size_t> vertex_labels( tGraph.sampleGraph.numberOfVertices() );
    edgeToVertexLabels( tGraph.sampleGraph, edge_labels, vertex_labels );

	cout<<"Done.\n";
	nClusters = *max_element(vertex_labels.begin(), vertex_labels.end()) + 1;
	cout << "Number of clusters: " << nClusters << endl;

	for(int i = 0; i < vertex_labels.size(); i++)
	{
		model<float, float>::Line* tLine = obj.lines[i];
		tLine->clusterID = vertex_labels[i];
	}

	colors.push_back( vec3( 0.5, 0.0, 0.0 ) );
	colors.push_back( vec3( 0.0, 0.5, 0.0 ) );
	colors.push_back( vec3( 0.0, 0.0, 0.5 ) );

	initialFlowLineExtraction(obj);

	cout << "Number of FlowLines: " << nFlowLines << endl;

	genReliableStrands( initialFlowLines );
	glLineWidth(3.0);

	//obj.updateModelFlowlines(  );
	obj.updateModelInitial(  );

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * obj.positions.size(), obj.positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, IBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * obj.ids.size(), obj.ids.data(), GL_DYNAMIC_DRAW);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, 0);
    glEnableVertexAttribArray(1);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * obj.indices.size(), obj.indices.data(), GL_STATIC_DRAW);*/

    program1.loadShaders("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl");
	glUseProgram(program1.id);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void setDraw()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * obj.positions.size(), obj.positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, IBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * obj.ids.size(), obj.ids.data(), GL_DYNAMIC_DRAW);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, 0);
    glEnableVertexAttribArray(1);
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program1.id);
	view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(glGetUniformLocation(program1.id, "view"), 1, GL_FALSE, &view[0][0]);
	mat4 projection = perspective(radians(45.0f), (float)winwidth / (float)winheight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(program1.id, "projection"), 1, GL_FALSE, &projection[0][0]);

	mat4 model;
	model = scale(model, vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(glGetUniformLocation(program1.id, "model"), 1, GL_FALSE, value_ptr(model));

	setDraw();

	glDrawArrays( GL_LINES, 0, obj.positions.size());
	//glDrawElements( GL_LINES, obj.indices.size(), GL_UNSIGNED_INT, nullptr);


	//glDrawElements( GL_LINES, sizeof(vec2) * obj.indices.size(), GL_UNSIGNED_INT, nullptr);
    glutSwapBuffers();
}

void motionfunc(int x,int y)
{
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top
    lastX = x;
    lastY = y;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw1 += xoffset;
    pitch1 += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch1 > 89.0f)
        pitch1 = 89.0f;
    if (pitch1 < -89.0f)
        pitch1 = -89.0f;


    vec3 front;
    front.x = cos(radians(yaw1)) * cos(radians(pitch1));
    front.y = sin(radians(pitch1));
    front.z = sin(radians(yaw1)) * cos(radians(pitch1));
    cameraFront = normalize(front);
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key=='q' || key==27) exit(0);
    float cameraSpeed = 2.5f * deltaTime;
    switch (key)
    {
        case 'w': case 'W':
            cameraPos.x = cameraPos.x + 0.02 * cameraFront.x;
			cameraPos.y = cameraPos.y + 0.02 * cameraFront.y;
			cameraPos.z = cameraPos.z + 0.02 * cameraFront.z;
            break;
        case 'a': case 'A':
			cameraPos.x = cameraPos.x - 0.02 * normalize(cross(cameraFront, cameraUp)).x;
			cameraPos.y = cameraPos.y - 0.02 * normalize(cross(cameraFront, cameraUp)).y;
			cameraPos.z = cameraPos.z - 0.02 * normalize(cross(cameraFront, cameraUp)).z;
            //cameraPos -= normalize(cross(cameraFront, cameraUp)) * 0.2;
            break;
        case 's': case 'S':
			cameraPos.x = cameraPos.x - 0.02 * cameraFront.x;
			cameraPos.y = cameraPos.y - 0.02 * cameraFront.y;
			cameraPos.z = cameraPos.z - 0.02 * cameraFront.z;
            break;
        case 'd': case 'D':
			cameraPos.x = cameraPos.x + 0.02 * normalize(cross(cameraFront, cameraUp)).x;
			cameraPos.y = cameraPos.y + 0.02 * normalize(cross(cameraFront, cameraUp)).y;
			cameraPos.z = cameraPos.z + 0.02 * normalize(cross(cameraFront, cameraUp)).z;
            //cameraPos += normalize(cross(cameraFront, cameraUp)) * 0.2;
            break;
		case '1':
			obj.updateModelInitial();
			break;
		case '2':
			obj.updateModelClusters();
			break;
		case '3':
			obj.updateModelClusters( 0 );
			break;
		case '4':
			obj.updateModelFlowlines();
			break;
		case '5':
			obj.updateModelFlowlines( 0 );
			break;
		case 'o': case 'O':
			glLineWidth(0.1);
			break;
		case 'p': case 'P':
			glLineWidth(3);
			break;
    }
    //std::cout<<cameraPos<<std::endl;
    //std::cout<<0.05 * cameraFront<<std::endl;
    glutPostRedisplay();
}

int main (int argc,char** argv)
{
    winwidth=winheight=512;

    glutInit(&argc,argv);
    glutInitWindowSize(winwidth,winheight);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("FlowRep");

    if ( glewInit() != GLEW_OK )
    {
        return -1;
    }

    init();
	glutDisplayFunc(display);
    glutReshapeFunc(reshapefunc);
    glutIdleFunc(display);
	glutKeyboardFunc(keyboardfunc);
	glutMotionFunc(motionfunc);
	glutMainLoop();
}
