#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include "Maths.h"
#include <glm.hpp>
#include <ext.hpp>
 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLCall.h"
 
using namespace std;
using namespace glm;

#define TYPE_NULL 0
#define TYPE_PROJECTION 1
#define TYPE_MODEL 2
#define TYPE_VIEW 3


class Shader {
private:
	string vertexContent;
	string fragmentContent;
	string geometryContent; //might fail
	unsigned int shader_Program;
	//list<string> inputTypes;
	string ReadShader(const char* path);
	unsigned int CompileShader(unsigned int type, const char* source);
	unsigned int CreateShaderProgram(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
public:
	Shader(const char* vertexPath, const char* fragmentPath,const char* geomPath,bool makeDefault);
	//Should not use!
	Shader(const char* shaderPath, bool makeDefault);
	Shader();
	~Shader();
	void UseShader();
	//Mem leak?
	void Reload();
	//Type should be GL_FLOAT_MAT4,GL_FLOAT_VEC3 and the like
	void UniformEquals(const char* uniform_Name, unsigned int type, void* value);
	//Don't use!
	void UniformMatrix(string uniform_Name,mat4* matrix,unsigned int type);
	//Don't use!
	void UniformVector(string uniform_Name, vec3* vec);
};
extern Shader* defaultShader;