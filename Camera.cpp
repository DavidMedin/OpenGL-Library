#include "pch.h"
#include "Camera.h"

std::list<Camera*> camList;

Camera::Camera(vec3* pos) {
	transform = new mat4(1);
	mat4* tmpMat = transform;

	orien = new quat(angleAxis<float>(0,vec3(0,1,0)));
	transform = new mat4(translate<float>(*transform, *pos));
	delete pos;
	delete tmpMat;
	camList.push_back(this);
	projectionMatrix = nullptr;
	fov = NULL;
	nearRange = NULL;
	farRange = NULL;
	viewMat = nullptr;
}
Camera::Camera(vec3 pos) {
	transform = new mat4(1);
	mat4* tmpMat = transform;
	orien = new quat(angleAxis<float>(0, vec3(0, 1, 0)));

	transform = new mat4( translate<float>(*transform, pos));
	delete tmpMat;
	camList.push_back(this);
	
	projectionMatrix = nullptr;
	fov = NULL;
	nearRange = NULL;
	farRange = NULL;
	viewMat = nullptr;
}
Camera::~Camera() {
	for (list<Camera*>::iterator i = camList.begin(); i != camList.end(); i++) {
		if (*i == this) {
			camList.erase(i);
			break;
		}
	}
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
	quat* tmpQuat = orien;
	orien = new quat(newRot * (*orien));
	delete tmpQuat;
}
void Camera::Rotate(float angle,vec3* axis) {
	NewError("rotate is broken\n");
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


void Camera::RecalculateProjection() {
	NewProjection(fov, nearRange, farRange);
	//delete projectionMatrix;
	//projectionMatrix = new mat4(translate(identity<mat4>(),vec3(1,0,0)));
	
}
void Camera::NewProjection(double fov, double nearRange, double farRange) {
	int height, width;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	double aspect = (double)width / (double)height;
	double range = tan((double)fov) * 0.5 * nearRange;
	double Sx = (2 * nearRange) / (range * aspect + range * aspect);
	double Sy = nearRange / range;
	double Sz = -(farRange + nearRange) / (farRange - nearRange);
	double Pz = -(2 * farRange * nearRange) / (farRange - nearRange);
	double data[] = {
		Sx,0,0,0,
		0,Sy,0,0,
		0,0,Sz,-1,
		0,0,Pz,0
	};
	mat4* matrix = new mat4(make_mat4(data));

	if (projectionMatrix != nullptr) delete projectionMatrix;
	//projectionMatrix = new mat4(perspectiveFov<double>(fov, width, height, nearRange, farRange));
	projectionMatrix = matrix;
	this->fov = fov;
	this->nearRange = nearRange;
	this->farRange = farRange;
}
GRAPHICSLIBRARY_API list<Camera*>* GetCameraList() {
	return &camList;
}