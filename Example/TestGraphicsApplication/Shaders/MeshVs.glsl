#version 460

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in int boneId;
//uniform vec3 camLoc;
uniform mat4 proj, view, model, bones[32];

layout(std140) buffer Test
{
	vec3 vector;
	float value[2];
	mat4 matrix;
	vec4 vecs[3];
	float target[];
};


out vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;

	vec3 idColor;
}v_frag;

void main() {
	int extra=0;
	for(int i = 0;i < target.length();i++){
		extra+=target[i];
	}
	if(boneId != -1){
		gl_Position = proj * view * model * bones[boneId] * vec4(vp+ vec3(extra,0,0),1.0);
	}else{
		gl_Position = proj * view * model * vec4(vp, 1.0);
	}
	v_frag.normals = normals;
	v_frag.texCoords = texCoords;
	v_frag.fragLoc = model * vec4(vp,1);

	if(boneId!=0){
		v_frag.idColor = vector + vec3(value[0],value[1],0);
	}else{
		v_frag.idColor = vec3(vecs[0]*vecs[1]);
	}
}