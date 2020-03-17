#version 420
layout(location = 0) in vec3 vp;
uniform mat4 proj,view,model;


void main(){
	gl_Position = proj * view * model * vec4(vp,1.0);
}