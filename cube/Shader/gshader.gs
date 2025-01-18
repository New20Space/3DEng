#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
out vec3 Color;


void main() {  
	vec4 line1,line2;

	line1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	line2 = gl_in[2].gl_Position - gl_in[0].gl_Position;
	


	vec3 normal = normalize(cross(line1.xyz, line2.xyz));

	vec3 lPos = vec3(20.0,40.0,-30);

    vec3 L = normalize(lPos-gl_in[0].gl_Position.xyz);
	Color = vec3(0.0, 1.0, 0.0)* vec3(dot(L,normal),dot(L,normal),dot(L,normal));

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	L = normalize(lPos-gl_in[1].gl_Position.xyz);
	Color = vec3(0.0, 1.0, 0.0)* vec3(dot(L,normal),dot(L,normal),dot(L,normal));

	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	L = normalize(lPos-gl_in[2].gl_Position.xyz);
	Color = vec3(0.0, 1.0, 0.0)* vec3(dot(L,normal),dot(L,normal),dot(L,normal));

	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();

}  