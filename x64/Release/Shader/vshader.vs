#version 330
#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Col;
layout (location = 2) in vec3 Normal;
out vec3 Color;

uniform mat4 gWorld;
uniform float f;



uniform vec2 Sp1;
uniform float array[90];
uniform float sc;



void main()
{
   

    //Color = Col;
    //vec3(0.0,0.0,0.7+0.1*(cos(M_PI*2/2*Position.z+M_PI*2/2*Position.x)));

    vec3 lPos = vec3(100,100,100);
    vec3 L = normalize(lPos-Position.xyz);

    
    vec3 dif =vec3(dot(L,Normal),dot(L,Normal),dot(L,Normal));
   
    Color = (dif+1)*0.2+Col*0.6;//mix(vec3(1,1,1),,exp(-0.2)) ;

    gl_Position = gWorld * vec4(Position.xyz,1.0);


     
}