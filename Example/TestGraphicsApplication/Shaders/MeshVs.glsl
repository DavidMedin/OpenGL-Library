#version 460
//vertex buffers
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;
//uniforms
layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;
layout(location = 4) uniform mat4 identity;
layout(location = 5) uniform mat4 bones[32];


layout(location = 0) out vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;

	vec3 idColor;
}v_frag;

void main() {

	mat4 finalBoneMat=identity;
	for(int i = 0; i < 4;i++){
		if(boneIds[i] != -1){
			finalBoneMat *= bones[boneIds[i]];
		}
	}
	if(boneIds[0] != -1){
		finalBoneMat *= bones[boneIds[0]];
	}
	gl_Position = proj * view * model * finalBoneMat * vec4(vp,1.0);
	v_frag.normals = normals;
	v_frag.texCoords = texCoords;
	v_frag.fragLoc = model * vec4(vp,1);


	int worthless=1;
	for(int i = 0;i < 4;i++){
		if(boneIds[i] != -1) {
			worthless=0;
			break;
		}
	}

	if(worthless==1){
		v_frag.idColor = vec3(1,0,0);
	}else{
		v_frag.idColor = vec3(0,1,0);
	}
}