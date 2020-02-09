#version 450

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

//uniform vec3 camLoc;
uniform mat4 view, model;

out vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;
}v_frag;

void main() {
	gl_Position = view * model * vec4(vp, 1.0);
	v_frag.normals = normals;
	v_frag.texCoords = texCoords;
	v_frag.fragLoc = model * vec4(vp, 1);
}
//@vertex



#version 450
#extension GL_NV_gpu_shader5 : enable
//const vec4 bitEnc = vec4(1., 255., 65025., 16581375.);
//const vec4 bitDec = 1. / bitEnc;
//vec4 EncodeFloatRGBA(float v) {
//	vec4 enc = bitEnc * v;
//	enc = fract(enc);
//	enc -= enc.yzww * vec2(1. / 255., 0.).xxxy;
//	return enc;
//}
//float DecodeFloatRGBA(vec4 v) {
//	return dot(v, bitDec);
//}


in vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;
}v_frag;
uniform vec3 camLoc;
uniform float maxClip;
out vec4 frag_colour;
void main() {
	//frag_colour = v_frag.fragLoc;
	//boi oh boi a comment!~
	int voxelDepth = 32 * 4; //32 for r g b and a
	int depth = int(distance(vec4(camLoc, 1), v_frag.fragLoc));
	float factor = float(voxelDepth) / maxClip;
	//assumes camera is at 0,0,0 (after view so should be fine)

	int varIndex = int(ceil(float(depth) * factor / 32) - 1); //32 is size of a variable (r or g ...)
	int dif = varIndex * 32 - int(float(depth) * factor);
	//need to write 1 to the correct bit
	//check to see if 32 is correct
	int32_t oneBit = 2147483648;//should be 10000000000000000000000000000000 - 31 zeros btw
	//float oneBit = 1;
	//for (int i = 0; i < dif; i++) {
	//	oneBit = DecodeFloatRGBA(EncodeFloatRGBA(oneBit));
	//}
	oneBit = oneBit >> dif;
	switch (varIndex) {
	case 1: {
		frag_colour = vec4(0, 0, 0, float(oneBit));
		return;
	}
	case 2: {
		frag_colour = vec4(0, 0, float(oneBit), 0);
		return;
	}
	case 3: {
		frag_colour = vec4(0, float(oneBit), 0, 0);
		return;
	}
	case 4: {
		frag_colour = vec4(float(oneBit), 0, 0, 0);
		return;
	}
	}
	frag_colour = vec4(.5,.5,.5, 1);
	return;
}
//@fragment