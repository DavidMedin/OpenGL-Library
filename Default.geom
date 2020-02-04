#version 420
//struct vData{
//	vec3 normals;
//	vec2 texCoords;
//};
//
layout(triangles) in;
in vec2 inData[];

out vec2 outData;
layout(triangle_strip,max_vertices  = 6) out;
void main(){
	//assume only points
	for(int i = 0;i < gl_in.length();i++){
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
//		outData.normals = inData[i].normals;
		outData = inData[i];
		EndPrimitive();

	}
}