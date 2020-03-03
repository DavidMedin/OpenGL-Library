#version 420
uniform vec3 ambientColor;
uniform vec3 lightPos; // will be only the closest light source (make a transition light animation like tww)
uniform vec3 lightColor;

uniform vec2 LightRamp1;
uniform vec2 LightRamp2;


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
	float diff = max(dot(norm, lightPos), 0.0); // light num
	
	vec2 LeftTarget;
	vec2 RightTarget;
	vec2 LightRamp0 = vec2(0,0);
	vec2 LightRamp3 = vec2(1,1);
	vec2 LightRampArray[] = {LightRamp0,LightRamp1,LightRamp2,LightRamp3};

	for(int i = 3;i > -1;i--){
		if(LightRampArray[i].x < diff){
			LeftTarget = LightRampArray[i];
			break;
		}
	}
	for(int q=0;q < 4; q++){
		if(LightRampArray[q].x>diff){
			RightTarget = LightRampArray[q];	
			break;
		}
	} 
	float slope = RightTarget.y-LeftTarget.y/RightTarget.x-LeftTarget.x;
	diff = slope * diff + LeftTarget.y;
	
	vec3 diffuseColor = diff * lightColor;
	vec4 result = vec4(ambientColor + diffuseColor, 1);
	frag_colour = result * texture(diffuse, v_frag.texCoords);
//	frag_colour = vec4(diff,diff,diff,1);
}