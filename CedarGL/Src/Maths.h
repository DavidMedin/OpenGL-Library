#pragma once
#include <stdio.h>
#include <iostream>

//glm
#include <glm.hpp>
#include <ext.hpp>
#include "Error.h"



void Translate(glm::mat4* matrix, std::initializer_list<float> offset);
void Translate(glm::mat4* matrix, glm::vec3* offset);
//might not work
glm::vec3 TranslateVec(glm::vec3* input, glm::vec3 offset);
glm::mat4* NewProjection(double fov, double nearRange, double farRange);
glm::vec3 GetRightAxis(glm::quat* quater);