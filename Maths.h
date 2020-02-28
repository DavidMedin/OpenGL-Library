#pragma once
#include <stdio.h>
#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <ext.hpp>
#include "Error.h"
using namespace glm;



void Translate(mat4* matrix, std::initializer_list<float> offset);
void Translate(mat4* matrix, vec3* offset);
//might not work
vec3 TranslateVec(vec3* input, vec3 offset);
mat4* NewProjection(double fov, double nearRange, double farRange);
vec3 GetRightAxis(quat* quater);