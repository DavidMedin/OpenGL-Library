#version 420
uniform vec3 ambientColor;
uniform vec3 lightPos; // will be only the closest light source (make a transition light animation like tww)
uniform vec3 lightColor;

uniform vec2 LightRamp1;
uniform vec2 LightRamp2;

uniform int renderSwitch;
in vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;
}v_frag;
uniform vec3 camLoc;
layout(binding = 0) uniform sampler2D diffuse;

out vec4 frag_colour;

float Dither(float diff){
	float dithX = sin(gl_FragCoord.x);
	float dithY = sin(gl_FragCoord.y);
	float ditherDiff;
	if(dithX >= 0){
		if(dithY>=0 && diff==1){
			ditherDiff = 1;
		}else if(dithY<0 && diff>.25){
			ditherDiff = 1;
		}else ditherDiff = 0;}
	else{
		if(dithY>0 && diff >.5){
			ditherDiff = 1;
		}else if(dithY<=0 && diff >.75){
			ditherDiff = 1;
		}else ditherDiff = 0;
	}
	return ditherDiff;
}

float ColorRamp(vec2 _LightRamp1,vec2 _LightRamp2,float diff){

	vec2 LeftTarget;
	vec2 RightTarget;
	vec2 LightRamp0 = vec2(0,0);
	vec2 LightRamp3 = vec2(1,1);
	vec2 LightRampArray[] = {LightRamp0,_LightRamp1,_LightRamp2,LightRamp3};

	for(int i=3;i >-1;i--){
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
	float slope = (RightTarget.y-LeftTarget.y)/(RightTarget.x-LeftTarget.x);
	float newDiff = slope * diff + LeftTarget.y;
	return newDiff;
}

void main() {
	vec3 norm = normalize(v_frag.normals);
	vec3 lightPos = normalize(lightPos - v_frag.fragLoc.xyz);
	float oldDiff = max(dot(norm, lightPos), 0.0); // light num
	
	
	float diff = ColorRamp(LightRamp1,LightRamp2,oldDiff);
	float ditherDiff = Dither(diff);
	float pureDitherDiff = Dither(oldDiff);


	vec3 toonDiffuse = diff * lightColor;
	vec4 toonResult = vec4(ambientColor + toonDiffuse,1);
	
	vec3 ditherDiffuse = ditherDiff * lightColor;
	vec4 ditherResult = vec4(ambientColor + ditherDiffuse, 1);
	
	vec3 pureDitherDiffuse = pureDitherDiff * lightColor;
	vec4 pureDitherResult = vec4(ambientColor + pureDitherDiffuse,1);

	if(renderSwitch == 0){
		frag_colour = ditherResult * texture(diffuse, v_frag.texCoords);
	}else if(renderSwitch == 1){
		frag_colour = toonResult * texture(diffuse,v_frag.texCoords);
	}else if(renderSwitch == 2){
		frag_colour = pureDitherResult * texture(diffuse,v_frag.texCoords);
	}
}