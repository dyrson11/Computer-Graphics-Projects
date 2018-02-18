cmake_minimum_required( VERSION 3.0.0 )

project( finalProject )

# FIND LIBRARIES
find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLUT REQUIRED )

set ( CMAKE_CXX_STANDARD 11 )


include_directories( headers/ )

add_executable( test.out
                main.cpp
                src/graph.cpp
                src/model.cpp
                src/program.cpp )

target_link_libraries( test.out GL GLU glut GLEW )
