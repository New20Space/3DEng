#version 330

layout (location = 0) in vec6 Position;
out vec3 Color;

uniform mat4 gWorld;
uniform float f;
void main()
{
    //Color = vec3(0.7,0.3,0.1+0.7*(sin(4*Position.x+f)+sin(4*Position.z+f)));
   
    gl_Position = gWorld * vec4(Position.xyz,1.0);
    Color = vec3(0.7*-0.1*(gl_Position.z-25),0.3*-0.1*(gl_Position.z-25),0.1*-0.1*(gl_Position.z-25));
    //gl_Position = gWorld *vec4(Position.x,Position.y+0.5*(sin(4*Position.x+f)+sin(4*Position.z+f)),Position.z, 1.0);
    
     
}