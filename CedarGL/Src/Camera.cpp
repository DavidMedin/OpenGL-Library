#include "Camera.h"
//glew
#include <GL/glew.h>
//glfw
#include <GLFW/glfw3.h>
namespace Cedar {
	//it would be good if we got rid of this
	std::list<Camera*> camList;


	glm::vec3 Camera::GetTranslateVec() {
		return glm::vec3((*transform)[3][0], (*transform)[3][1], (*transform)[3][2]);
	}

	Camera::Camera(glm::vec3* pos) {
		transform = new glm::mat4(1);
		glm::mat4* tmpMat = transform;

		orien = new glm::quat(glm::angleAxis<float>(0, glm::vec3(0, 1, 0)));
		transform = new glm::mat4(glm::translate<float>(*transform, *pos));
		delete pos;
		delete tmpMat;
		camList.push_back(this);
		projectionMatrix = nullptr;
		fov = NULL;
		nearRange = NULL;
		farRange = NULL;
		viewMat = nullptr;
	}
	Camera::Camera(glm::vec3 pos) {
		transform = new glm::mat4(1);
		glm::mat4* tmpMat = transform;
		orien = new glm::quat(glm::angleAxis<float>(0, glm::vec3(0, 1, 0)));

		transform = new glm::mat4(glm::translate<float>(*transform, pos));
		delete tmpMat;
		camList.push_back(this);

		projectionMatrix = nullptr;
		fov = NULL;
		nearRange = NULL;
		farRange = NULL;
		viewMat = nullptr;
	}
	Camera::~Camera() {
		for (std::list<Camera*>::iterator i = camList.begin(); i != camList.end(); i++) {
			if (*i == this) {
				camList.erase(i);
				break;
			}
		}
	}
	void Camera::UpdateViewMatrix() {
		delete viewMat;
		glm::mat4* T = new glm::mat4(glm::translate(glm::identity<glm::mat4>(), glm::vec3(-(*transform)[3][0], -(*transform)[3][1], -(*transform)[3][2])));
		//mat4* T = new mat4(identity<mat4>());
		//mat4* R = new mat4(mat4_cast(angleAxis(-angle(*orien),axis(*orien))));
		glm::mat4* R = new glm::mat4(glm::transpose(mat4_cast(normalize(*orien))));

		//mat4* R = new mat4(mat4_cast(*orien));
		glm::mat4* eyeTmp = new glm::mat4(*R * *T);
		delete R;
		delete T;
		viewMat = eyeTmp;
	}
	void Camera::UpdateViewMatrix(glm::mat4& rotation) {
		delete viewMat;
		glm::mat4* T = new glm::mat4(glm::translate(glm::identity<glm::mat4>(), glm::vec3(-(*transform)[3][0], -(*transform)[3][1], -(*transform)[3][2])));
		//mat4* T = new mat4(identity<mat4>());

		//mat4* R = new mat4(mat4_cast(angleAxis(-angle(*orien),axis(*orien))));

		//mat4* R = new mat4(mat4_cast(*orien));
		glm::mat4* eyeTmp = new glm::mat4(rotation * *T);
		delete T;
		viewMat = eyeTmp;
	}
	void Camera::Translate(const glm::vec3& offset) {

		glm::mat4* tmpMat = transform;
		transform = new glm::mat4(translate(*tmpMat, offset));
		delete tmpMat;

	}
	void Camera::Translate(glm::vec3* offset) {
		glm::mat4* tmpMat = transform;
		transform = new glm::mat4(translate(*tmpMat, *offset));
		delete tmpMat;
	}
	void Camera::Rotate(float angle, const glm::vec3& axis) {


		glm::quat newRot = glm::angleAxis(glm::radians(angle), axis);
		glm::quat* tmpQuat = orien;
		orien = new glm::quat(newRot * (*orien));
		delete tmpQuat;
	}
	void Camera::Rotate(float angle, glm::vec3* axis) {
		NewError("rotate is broken\n");
		/*quat* tmpQuat = orien;
		orien = new quat(rotate<float>(*tmpQuat, radians(angle), *axis));
		delete tmpQuat;*/
	}

	void Camera::PrintTransform() {
		std::cout << glm::to_string(*transform) << "\n";
	}
	glm::vec3 Camera::GetRightAxis() {
		//UpdateViewMatrix();
		glm::vec3 forward = *orien * glm::vec3(0, 0, -1);
		forward = glm::vec3(forward.x, 0, forward.z);
		glm::vec3 right = cross(forward, glm::vec3(0, 1, 0));
		right = normalize(right);
		return right;
	}
	glm::vec3 Camera::GetForwardAxis() {
		UpdateViewMatrix();
		glm::mat4 mat = mat4_cast(*orien);
		glm::vec3 tmpVec = glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
		return tmpVec;
	}
	glm::vec3 Camera::GetUpAxis() {
		UpdateViewMatrix();
		glm::mat4 mat = glm::mat4_cast(*orien);
		glm::vec3 tmpVec = glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
		return tmpVec;
	}

	float Camera::GetY()
	{
		glm::vec3 flatDir = *orien * glm::vec3(0, 0, -1);
		flatDir.y = 0;
		flatDir = normalize(flatDir);
		return glm::degrees(atan2(flatDir.z, flatDir.x));
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
		double range = tan(glm::radians(fov)) * 0.5 * nearRange;
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
		glm::mat4* matrix = new glm::mat4(glm::make_mat4(data));

		if (projectionMatrix != nullptr) delete projectionMatrix;
		//projectionMatrix = new mat4(perspectiveFov<double>(fov, width, height, nearRange, farRange));
		projectionMatrix = matrix;
		this->fov = fov;
		this->nearRange = nearRange;
		this->farRange = farRange;
	}
	std::list<Camera*>* GetCameraList() {
		return &camList;
	}
}