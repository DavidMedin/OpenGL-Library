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
//@vertex



#version 420
uniform vec3 ambientColor;
uniform vec3 lightPos; // will be only the closest light source (make a transition light animation like tww)
uniform vec3 lightColor;
in vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;
}v_frag;
uniform vec3 camLoc;
layout(binding = 0) uniform sampler2D diffuse;

out vec4 frag_colour;
void main() {
	vec3 norm = normalize(v_frag.normals);
	vec3 lightPos = normalize(lightPos - v_frag.fragLoc.xyz);
	float diff = max(dot(norm, lightPos), 0.0);
	vec3 diffuseColor = diff * lightColor;
	vec4 result = vec4(ambientColor + diffuseColor, 1);
	frag_colour = result * texture(diffuse, v_frag.texCoords);
	//frag_colour = vec4(v_frag.normals,1);
}
//@fragment