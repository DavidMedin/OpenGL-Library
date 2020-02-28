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

GRAPHICSLIBRARY_API vec3 TranslateVec(vec3* input, vec3 offset)
{
	return *input + offset;
}

GRAPHICSLIBRARY_API mat4* NewProjection(double fov, double nearRange, double farRange) {
	int height, width;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	//mat4* matrix = new mat4(perspectiveFov<double>(radians(fov), width, height, nearRange, farRange));
	//double right = tan(fov) * nearRange * 0.5f; //not used
	//double top = tan(fov) * nearRange * 0.5f * (height/width);
	//double angleTop = atan2(top, nearRange);
	//mat4* matrix = new mat4(perspective<double>(angleTop, double(width/height), nearRange, farRange));
	double aspect = (double)height / (double)width;
	double range = tan((double)fov)*0.5 * nearRange;
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
	return matrix;
}
GRAPHICSLIBRARY_API vec3 GetRightAxis(quat* quater) {
	mat4 mat = mat4_cast(*quater);
	vec3 tmpVec = vec3(mat[0][0], mat[0][1], mat[0][2]);
	return tmpVec;
}