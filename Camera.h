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
public:
	mat4* transform;
	mat4* viewMat;
	quat* orien;
	Camera(std::initializer_list<float> pos);
	Camera(vec3* pos);
	void Translate(const vec3& offset);
	void Translate(vec3* offset);
	void Rotate(float angle,const vec3& axis);
	void Rotate(float angle,vec3* axis);
	void UpdateViewMatrix();
	void UpdateViewMatrix(mat4& rotation);
	void PrintTransform();
	vec3 GetRightAxis();
	vec3 GetForwardAxis();
	vec3 GetUpAxis();
};