#version 420

layout (location=0) in vec3 vp;
layout (location=1) in vec3 normals;
layout (location=2) in vec2 texCoords;

uniform mat4 proj,view,model;

out vec3 outNormals;
out vec2 outTexCoords;
out vec2 XY;
void main() {
	gl_Position = proj * view * model * vec4(vp,1.0);
	outNormals = normals;
	outTexCoords = texCoords;
	XY = vec2(gl_Position.xy);
}
//@vertex

#version 420
layout(points) in;
layout(triangle_strip,max_vertices  = 6) out;
void main(){
	//assume only points
	if (gl_in.length() == 1) {
		Gl_position = 
	}

}
//@geometry

#version 420

 in vec3 outNormals;
 in vec2 outTexCoords;
 in vec2 XY;

layout (binding = 0) uniform sampler2D diffuse;

out vec4 frag_colour;
void main() {
	frag_colour = texture(diffuse,outTexCoords);
	

}
//@fragment