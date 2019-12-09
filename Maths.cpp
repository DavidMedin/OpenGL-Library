#include "pch.h"
#include "Maths.h"


GRAPHICSLIBRARY_API void Translate(mat4* matrix, std::initializer_list<float> offset)
{
	matrix[4][0] += *(offset.begin());
	matrix[4][1] += *(offset.begin()+1);
	matrix[4][2] += *(offset.begin()+2);
}
GRAPHICSLIBRARY_API void Translate(mat4* matrix, vec3* offset)
{
	matrix[3][0] += offset->x;
	matrix[3][1] += offset->y;
	matrix[3][2] += offset->z;
}

GRAPHICSLIBRARY_API mat4* NewProjection(double fov, double nearRange, double farRange) {
	int height, width;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	/*mat4* matrix = new mat4(perspectiveFov<double>(fov, width, height, nearRange, farRange));*/
	mat4* matrix = new mat4(perspective<double>(fov, double(width/height), nearRange, farRange));
	return matrix;
}
GRAPHICSLIBRARY_API vec3 GetRightAxis(quat* quater) {
	mat4 mat = mat4_cast(*quater);
	vec3 tmpVec = vec3(mat[0][0], mat[0][1], mat[0][2]);
	return tmpVec;
}