#pragma once
#include "Maths.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/quaternion.hpp>
#include <matrix.hpp>
#include <ext.hpp>
#include <GL/glew.h>


class  Camera {
private:
	double fov;
	double nearRange, farRange;

public:
	glm::mat4* transform;
	glm::quat* orien;
	
	glm::mat4* viewMat;
	glm::mat4* projectionMatrix;


	Camera(glm::vec3* pos);
	Camera(glm::vec3 pos);
	~Camera();

	void Translate(const glm::vec3& offset);
	void Translate(glm::vec3* offset);
	void Rotate(float angle,const glm::vec3& axis);
	void Rotate(float angle,glm::vec3* axis);
	void UpdateViewMatrix();
	void UpdateViewMatrix(glm::mat4& rotation);
	void PrintTransform();
	glm::vec3 GetRightAxis();
	//might not work
	glm::vec3 GetForwardAxis();
	//might not work
	glm::vec3 GetUpAxis();
	float GetX();
	float GetY();
	float GetZ();

	void RecalculateProjection();
	void NewProjection(double fov, double nearRange, double farRange);
	glm::vec3 GetTranslateVec();
};

//GRAPHICSLIBRARY_API mat4* GetCurrentProjection();
//GRAPHICSLIBRARY_API void SetCurrentProjection(mat4* proj);
 std::list<Camera*>* GetCameraList();