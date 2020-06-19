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
//layout(location = 2) uniform mat4 model;
layout(location = 2) uniform mat4 bones[32];


layout(location = 0) out vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;

	vec3 idColor;
}v_frag;

void main() {
	bool hasBones=false;

	vec4 finalPos = vec4(0);
	vec3 finalNormal = vec3(0);
	for(int i = 0; i < 4;i++){
		if(boneIds[i] != -1){
			finalPos += (bones[boneIds[i]]*vec4(vp,1) * weights[i]);
			finalNormal += (bones[boneIds[i]]*vec4(normals,1)*weights[i]).xyz;
			hasBones=true;
		}
	}

	if(hasBones){
		gl_Position = proj * view * finalPos;
		v_frag.normals = finalNormal;
	}else{
		gl_Position = proj* view * vec4(vp,1);
		v_frag.normals = normals;
	}
	v_frag.texCoords = texCoords;
	v_frag.fragLoc = vec4(vp,1);

}