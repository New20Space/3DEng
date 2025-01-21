#version 330
#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Col;
layout (location = 2) in vec3 Normal;
out vec3 Color;

uniform mat4 gWorld;
uniform mat4 gNormal;
uniform vec3 LightPos;
uniform float f;



uniform vec2 Sp1;
uniform float array[90];
uniform float sc;



void main()
{

    Color = Col;

    gl_Position = gWorld * vec4(Position.xyz,1.0);


     
}