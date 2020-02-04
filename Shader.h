#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include "Maths.h"
#include <glm.hpp>
#include <ext.hpp>
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glew.h>
#include <glfw3.h>
#include "GLCall.h"
#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
using namespace std;
using namespace glm;

#define MATRIX4 1
#define VEC3F 2
#define TYPE_NULL 0
#define TYPE_PROJECTION 1
#define TYPE_MODEL 2
#define TYPE_VIEW 3


class GRAPHICSLIBRARY_API Shader {
private:
	unsigned int shader_Program;
	//list<string> inputTypes;
	string ReadShader(const char* path);
	unsigned int CompileShader(unsigned int type, const char* source);
	unsigned int CreateShaderProgram(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
public:
	Shader(const char* vertexPath, const char* fragmentPath,const char* geomPath,bool makeDefault);
	Shader(const char* shaderPath, bool makeDefault);
	~Shader();
	void UseShader();
	//void UniformEquals(const char* uniform_Name, unsigned int type, float* value);
	/*void UniformMatrix(string uniform_Name,Matrix4* matrix);
	void UniformVector(string uniform_Name, Vector3* vec);*/
	void UniformMatrix(string uniform_Name,mat4* matrix,unsigned int type);
	void UniformVector(string uniform_Name, vec3* vec);
};
extern Shader* defaultShader;