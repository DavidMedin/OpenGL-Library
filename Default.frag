#version 420
//
//struct vData{
//	vec3 normals;
//	vec2 texCoords;
//};
in vec2 inData;

layout (binding = 0) uniform sampler2D diffuse;

out vec4 frag_colour;
void main() {
	frag_colour = texture(diffuse,inData);
//    frag_colour = vec4(1,1,1,1);
}