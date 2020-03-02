#version 420

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

//uniform vec3 camLoc;
uniform mat4 proj, view, model;

out vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;
}v_frag;

void main() {
	gl_Position = proj * view * model * vec4(vp, 1.0);
	v_frag.normals = normals;
	v_frag.texCoords = texCoords;
	v_frag.fragLoc = model * vec4(vp,1);
}