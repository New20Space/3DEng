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

mat3 m4_m3(mat4 m4) {
  return mat3(
      m4[0][0], m4[0][1], m4[0][2],
      m4[1][0], m4[1][1], m4[1][2],
      m4[2][0], m4[2][1], m4[2][2]);
}

void main()
{
	vec3 NPosition = Position;
	vec3 NNormal = m4_m3(gNormal)*Normal;
   

    //Color = Col;
    //vec3(0.0,0.0,0.7+0.1*(cos(M_PI*2/2*Position.z+M_PI*2/2*Position.x)));

    vec3 lPos = LightPos;
    vec3 L = normalize(lPos-NPosition.xyz);


    
    vec3 dif =vec3(dot(L,NNormal),dot(L,NNormal),dot(L,NNormal));
   
    Color = (dif+1)*0.2+Col*0.6;//mix(vec3(1,1,1),,exp(-0.2)) ;

    gl_Position = gWorld * vec4(Position.xyz,1.0);


     
}