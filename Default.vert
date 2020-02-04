#version 420

layout (location=0) in vec3 vp;
layout (location=1) in vec3 normals;
layout (location=2) in vec2 texCoords;

uniform mat4 proj,view,model;

out vec3 outNormals;
out vec2 outTexCoords;
void main() {
	gl_Position = proj * view * model * vec4(vp,1.0);
	outNormals = normals;
	outTexCoords = texCoords;
}