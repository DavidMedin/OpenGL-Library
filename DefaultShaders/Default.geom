#version 420

layout(triangles) in;

in vData{
	vec3 normals;
	vec2 texCoords;
}v_geom[];

out fData{
	vec3 normals;
	vec2 texCoords;
}v_frag;

layout(triangle_strip,max_vertices  = 6) out;
void main(){
	for(int i = 0;i < gl_in.length();i++){
		v_frag.texCoords = v_geom[i].texCoords;
		v_frag.normals = v_geom[i].normals;
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();


	}
		EndPrimitive();
}