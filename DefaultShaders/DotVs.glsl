#version 130
in vec3 pos;
uniform vec3 color;
uniform mat4 proj,view,model;

out vec3 colorOut;
void main(){
	gl_Position = proj * view * model * vec4(pos,1.0);
	colorOut = color;
}