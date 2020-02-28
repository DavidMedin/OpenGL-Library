#pragma once
#include <stdio.h>
#include <iostream>
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <ext.hpp>
#include "Error.h"
using namespace glm;
#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif


GRAPHICSLIBRARY_API void Translate(mat4* matrix, std::initializer_list<float> offset);
GRAPHICSLIBRARY_API void Translate(mat4* matrix, vec3* offset);
//might not work
GRAPHICSLIBRARY_API vec3 TranslateVec(vec3* input, vec3 offset);
GRAPHICSLIBRARY_API mat4* NewProjection(double fov, double nearRange, double farRange);
GRAPHICSLIBRARY_API vec3 GetRightAxis(quat* quater);