#version 330 core

layout(location = 0) out vec3 color;
in vec3 Color;



uniform vec3 u_Pos;
uniform float f;
uniform vec2 Sp;
uniform float M;
uniform float array[90];
uniform float sc;




void main(){


    color = Color;
    
}
/*
void main(){
	
   if(gl_FragCoord.x < array[0]*10){
        if(gl_FragCoord.y < array[1]*10)
            color = vec4(1.0, 0.0, 0.0, 1.0);
        else
            color = vec4(0.0, 1.0, 0.0, 1.0);
   }
   else
        color = vec4(0.0, 1.0, 0.0, 1.0);
    
}*/
