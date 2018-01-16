#version 120  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version


attribute vec2 vPosition;
//varying vec3 vColor;
//varying out vec4 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int BIT_COUNT = 8;

int modi(int x, int y)
{
    return x - y * (x / y);
}

int or(int a, int b)
{
    int result = 0, n = 1;
    for(int i = 0; i < BIT_COUNT; i++)
    {
        if ((modi(a, 2) == 1) || (modi(b, 2) == 1))
            result += n;
        a = a / 2;
        b = b / 2;
        n = n * 2;
        if(!(a > 0 || b > 0))
            break;
    }
    return result;
}

float findnoise2(float x, float y)
{
	int n = int(floor(x)) + int(floor(y)) * 57;
	n = or(n * 8192, n);
	int nn = (n*(n*n*60493+19990303)+1376312589);
	return 1.0 - (float(nn)/1073741824.0);
}

float interpolate1(float a,float b,float x)
{
	float ft=x * 3.1415927;
	float f=(1.0-cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}


float noise(float x,float y)
{
	float floorx = floor(x);//This is kinda a cheap way to floor a float integer.
	float floory = floor(y);
	float s, t, u, v;//Integer declaration
	s = findnoise2(floorx, floory);
	t = findnoise2(floorx + 1, floory);
	u = findnoise2(floorx, floory + 1);//Get the surrounding pixels to calculate the transition.
	v = findnoise2(floorx + 1, floory + 1);
	float int1 = interpolate1(s, t, x - floorx);//Interpolate between the values.
	float int2 = interpolate1(u, v, x - floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return interpolate1(int1, int2, y - floory);//Here we use y-floory, to get the 2nd dimension.
}

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, noise(vPosition.x/3, vPosition.y/3) / 5.0, vPosition.y,1.0);
    //gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, -1.0, vPosition.y,1.0);
    //color = vec4(1,1,1,1);
}
