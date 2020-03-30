#version 420

in vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;
}v_frag;

uniform sampler2D tex;

out vec4 outFrag;
void main(){

outFrag = texture(tex,v_frag.texCoords);
}