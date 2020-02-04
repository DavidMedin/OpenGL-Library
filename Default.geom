#version 420
layout(triangles) in;
in vec3 inNormals;
in vec2 inTexCoords;

out vec3 outNormals;
out vec2 outTexCoords;
layout(triangle_strip,max_vertices  = 6) out;
void main(){
	//assume only points
	for(int i = 0;i < gl_in.length();i++){
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	outNormals = inNormals;
	outTexCoords = inTexCoords;
}