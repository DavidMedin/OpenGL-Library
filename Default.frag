#version 420

 in vec3 outNormals;
 in vec2 outTexCoords;

layout (binding = 0) uniform sampler2D diffuse;

out vec4 frag_colour;
void main() {
	frag_colour = texture(diffuse,outTexCoords);
//    frag_colour = vec4(1,1,1,1);
}