#version 410

in vec3 outColor;
out vec4 frag_colour;
void main() {
	frag_colour = vec4(outColor,1.0);
}