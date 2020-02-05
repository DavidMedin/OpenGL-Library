#version 420

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

uniform mat4 proj, view, model;

out vData{
	vec3 normals;
	vec2 texCoords;
}v_geom;

void main() {
	gl_Position = proj * view * model * vec4(vp, 1.0);
	v_geom.normals = normals;
	v_geom.texCoords = texCoords;
}
//@vertex

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

layout(triangle_strip, max_vertices = 6) out;
void main() {
	for (int i = 0; i < gl_in.length(); i++) {
		v_frag.texCoords = v_geom[i].texCoords;
		v_frag.normals = v_geom[i].normals;
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();


	}
	EndPrimitive();
}
//@geometry

#version 420

in fData{
	vec3 normals;
	vec2 texCoords;
}v_frag;
layout(binding = 0) uniform sampler2D diffuse;

out vec4 frag_colour;
void main() {
	frag_colour = texture(diffuse, v_frag.texCoords);
	//    frag_colour = vec4(1,1,1,1);
}
//@fragment