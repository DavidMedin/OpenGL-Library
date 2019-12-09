#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <glm.hpp>
#include <mat4x4.hpp>
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glew.h>
#include <glfw3.h>

#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
#define ONE_DEG_IN_RAD (2.0 * (float)M_PI) / 360.0
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
struct mat4 {
	float m[16];
	void operator*= (const mat4* rhs);
	void operator=(const mat4* rhs);
	mat4(std::initializer_list<float> list);
	mat4(float* data);
	mat4();

	void PrintMat();
	void Transpose();
	void TranslateX(float dist);
	void TranslateY(float dist);
	void TranslateZ(float dist);
};
GRAPHICSLIBRARY_API mat4* ProjectionMatrix(float near, float far, float fov_in);
GRAPHICSLIBRARY_API glm::mat4 getGlmMat4(mat4* matrix);
GRAPHICSLIBRARY_API mat4 GetMat4(glm::mat4 matrix);