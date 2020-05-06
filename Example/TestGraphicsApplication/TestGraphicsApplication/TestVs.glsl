#version 440

layout(location = 0) in vec3 vp;
layout(location = 2) in vec2 texCoords;

uniform mat4 proj, view, model;

layout(std140) uniform Test
{
	vec3 vector;
	float value;
	mat4 matrix;
	float array[3];
	bool boolie;
	int integer;
};



out vec2 texCoord;
void main() {
	if(boolie==true){
		gl_Position = proj * view * model *matrix* vec4(vp+vector+vec3(0,value-integer,0), 1.0);
	}else gl_Position = proj*view * model * vec4(vp,1.0);
	texCoord = texCoords;
}