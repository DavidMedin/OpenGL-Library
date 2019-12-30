#version 410

in vec3 vp;
in vec3 normals;
uniform vec3 color;
uniform mat4 proj,view;
out vec3 outColor;
void main() {
	gl_Position = proj * view * vec4(vp,1.0);
	outColor = normals;
}