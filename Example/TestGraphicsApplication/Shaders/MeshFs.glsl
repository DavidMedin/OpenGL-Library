#version 460
layout(location=37) uniform vec3 ambientColor;
layout(location=38) uniform vec3 lightPos; // will be only the closest light source (make a transition light animation like tww)
layout(location=39) uniform vec3 lightColor;

layout(location=40) uniform vec2 LightRamp1;
layout(location=41) uniform vec2 LightRamp2;

layout(location=42) uniform int renderSwitch;
layout(location=43) uniform vec3 camLoc;
layout(location=0) in vData{
	vec3 normals;
	vec2 texCoords;
	vec4 fragLoc;

	vec3 idColor;
}v_frag;
layout(binding = 0) uniform sampler2D diffuse;

layout(location=0) out vec4 frag_colour;

float Dither(float diff,int rez){
	float dithX = sin(gl_FragCoord.x);
	float dithY = sin(gl_FragCoord.y);
	float ditherDiff=0;
	if(dithX >= 0){
		if(dithY>=0 && diff>=1){
			ditherDiff = 1;
		}else if(dithY<0 && diff>=.25){
			ditherDiff = 1;
		}else ditherDiff = 0;}
	else{
		if(dithY>=0 && diff >=.5){
			ditherDiff = 1;
		}else if(dithY<=0 && diff >=.75){
			ditherDiff = 1;
		}else ditherDiff = 0;
	}
//	for(int i=0; i < rez;i++){
//		if(dithX>=-1+(i*(2/rez))){
//			for(int u = 0;u < rez;u++){
//				if(dithY>=-1+(u*(2/rez)) && diff >= ){
//					
//				}
//			}
//		}
//	}
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
	if(newDiff > 1) newDiff = 1;
	return newDiff;
}

void main() {
	vec3 norm = normalize(v_frag.normals);
	vec3 lightPos = normalize(lightPos - v_frag.fragLoc.xyz);
	float oldDiff = max(dot(norm, lightPos), 0.0); // light num
	
	
	float diff = ColorRamp(LightRamp1,LightRamp2,oldDiff);
	float ditherDiff = Dither(diff,2);
	float pureDitherDiff = Dither(oldDiff,2);


	vec4 toonResult = vec4(ambientColor + (diff * lightColor),1);
	
	vec3 ditherDiffuse = ditherDiff * lightColor;
	vec4 ditherResult = vec4(ambientColor + ditherDiffuse, 1);
	
	vec3 pureDitherDiffuse = pureDitherDiff * lightColor;
	vec4 pureDitherResult = vec4(ambientColor + pureDitherDiffuse,1);

	if(renderSwitch == 0){
		frag_colour = ditherResult * texture(diffuse, v_frag.texCoords);
	}
	else if(renderSwitch == 1){
		frag_colour = toonResult * texture(diffuse,v_frag.texCoords);
	}else if(renderSwitch == 2){
		frag_colour = vec4(ambientColor,1) * texture(diffuse,v_frag.texCoords);
	}else if(renderSwitch == 3){
		frag_colour = texture(diffuse,v_frag.texCoords);
	}else if(renderSwitch == 4){
		frag_colour = vec4(v_frag.idColor,1);
	}
	if(texture(diffuse,v_frag.texCoords).a == 0){
		discard;
	}
}