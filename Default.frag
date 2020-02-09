#version 420

in vData{
	vec3 normals;
	vec2 texCoords;
}v_frag;
layout (binding = 0) uniform sampler2D diffuse;

out vec4 frag_colour;
void main() {
	

	frag_colour = texture(diffuse,v_frag.texCoords);
//    frag_colour = vec4(1,1,1,1);
//distance(vec3(1,1,1),vec3(1,1,1));
}