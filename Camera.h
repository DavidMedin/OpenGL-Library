#pragma once
#include "Maths.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/quaternion.hpp>
#include <matrix.hpp>
#include <ext.hpp>
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glew.h>
using namespace glm;

#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif

class GRAPHICSLIBRARY_API Camera {
private:
	double fov;
	double nearRange, farRange;

public:
	mat4* transform;
	quat* orien;
	
	mat4* viewMat;
	mat4* projectionMatrix;


	Camera(vec3* pos);
	Camera(vec3 pos);
	~Camera();

	void Translate(const vec3& offset);
	void Translate(vec3* offset);
	void Rotate(float angle,const vec3& axis);
	void Rotate(float angle,vec3* axis);
	void UpdateViewMatrix();
	void UpdateViewMatrix(mat4& rotation);
	void PrintTransform();
	vec3 GetRightAxis();
	//might not work
	vec3 GetForwardAxis();
	//might not work
	vec3 GetUpAxis();
	float GetX();
	float GetY();
	float GetZ();

	void RecalculateProjection();
	void NewProjection(double fov, double nearRange, double farRange);
	vec3 GetTranslateVec();
};

//GRAPHICSLIBRARY_API mat4* GetCurrentProjection();
//GRAPHICSLIBRARY_API void SetCurrentProjection(mat4* proj);
GRAPHICSLIBRARY_API list<Camera*>* GetCameraList();