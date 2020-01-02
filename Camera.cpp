#include "pch.h"
#include "Camera.h"


Camera::Camera(std::initializer_list<float> pos) {

	transform = new mat4(1.0f);
	//printf("%f\n", (*(transform))[3][1]);
	orien = new quat(glm::identity<quat>());
	viewMat = new mat4(1);
	//printf("%f,%f,%f\n",*pos.begin(),*(pos.begin()+1),*(pos.begin()+2));
	//translate<float>(*transform, vec3(*pos.begin(), *(pos.begin() + 1), *(pos.begin() + 2)));
	/*Translate(transform, { *pos.begin(),*(pos.begin() + 1),*(pos.begin() + 2) });*/
	mat4* tmpMat = transform;
	transform = new mat4(translate(*tmpMat, vec3( *pos.begin(),*(pos.begin() + 1),*(pos.begin() + 2) )));
	delete tmpMat;
}
Camera::Camera(vec3* pos) {
	transform = new mat4(1);
	orien = new quat(angleAxis<float>(0,vec3(0,1,0)));
	translate<float>(*transform, *pos);
	delete pos;
}
void Camera::UpdateViewMatrix() {
	delete viewMat;
	mat4* T = new mat4(translate(identity<mat4>(),vec3( -(*transform)[3][0], -(*transform)[3][1], -(*transform)[3][2])));
	//mat4* T = new mat4(identity<mat4>());
	//mat4* R = new mat4(mat4_cast(angleAxis(-angle(*orien),axis(*orien))));
	mat4* R = new mat4(glm::transpose(mat4_cast(normalize(*orien))));

	//mat4* R = new mat4(mat4_cast(*orien));
	mat4* eyeTmp = new mat4(*R * *T);
	delete R;
	delete T;
	viewMat = eyeTmp;
}
void Camera::UpdateViewMatrix(mat4& rotation) {
	delete viewMat;
	mat4* T = new mat4(translate(identity<mat4>(), vec3(-(*transform)[3][0], -(*transform)[3][1], -(*transform)[3][2])));
	//mat4* T = new mat4(identity<mat4>());

	//mat4* R = new mat4(mat4_cast(angleAxis(-angle(*orien),axis(*orien))));

	//mat4* R = new mat4(mat4_cast(*orien));
	mat4* eyeTmp = new mat4(rotation * *T);
	delete T;
	viewMat = eyeTmp;
}
void Camera::Translate(const vec3& offset) {

	mat4* tmpMat = transform;
	transform = new mat4(translate(*tmpMat, offset));
	delete tmpMat;

}
void Camera::Translate(vec3* offset) {
	mat4* tmpMat = transform;
	transform = new mat4(translate(*tmpMat, *offset));
	delete tmpMat;
}
void Camera::Rotate(float angle,const vec3& axis) {


	quat newRot = angleAxis(radians(angle), axis);
	//newRot = normalize(newRot);
	orien = new quat(newRot * (*orien));
	//orien = new quat(normalize(*orien));
}
void Camera::Rotate(float angle,vec3* axis) {
	/*quat* tmpQuat = orien;
	orien = new quat(rotate<float>(*tmpQuat, radians(angle), *axis));
	delete tmpQuat;*/
}

void Camera::PrintTransform() {
	cout << glm::to_string(*transform) << "\n";
}
vec3 Camera::GetRightAxis() {
	//UpdateViewMatrix();
	vec3 forward = *orien * vec3(0,0,-1);
	forward = vec3(forward.x, 0, forward.z);
	vec3 right = cross(forward,vec3(0,1,0));
	right = normalize(right);
	return right;
}
vec3 Camera::GetForwardAxis() {
	UpdateViewMatrix();
	mat4 mat = mat4_cast(*orien);
	vec3 tmpVec = vec3(mat[0][2], mat[1][2], mat[2][2]);
	return tmpVec;
}
vec3 Camera::GetUpAxis() {
	UpdateViewMatrix();
	mat4 mat = mat4_cast(*orien);
	vec3 tmpVec = vec3(mat[0][1], mat[1][1], mat[2][1]);
	return tmpVec;
}

float Camera::GetY()
{
	vec3 flatDir = *orien * vec3(0,0,-1);
	flatDir.y = 0;
	flatDir = normalize(flatDir);
	return degrees(atan2(flatDir.z,flatDir.x));
}