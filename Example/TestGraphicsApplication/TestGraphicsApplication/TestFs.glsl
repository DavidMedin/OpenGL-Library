#version 440
in 	vec2 texCoord;
layout(binding = 0) uniform sampler2D diffuse;

out vec4 color;
void main(){
	color = texture(diffuse,texCoord);
}