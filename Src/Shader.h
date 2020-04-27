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
 


#define TYPE_NULL 0
#define TYPE_PROJECTION 1
#define TYPE_MODEL 2
#define TYPE_VIEW 3


class Shader {
private:
	std::string vertexContent;
	std::string fragmentContent;
	std::string geometryContent; //might fail
	//list<string> inputTypes;
	unsigned int shader_Program;
	std::string ReadShader(const char* path);
	unsigned int CompileShader(unsigned int type, const char* source);
	unsigned int CreateShaderProgram(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
	void _UniformEquals(int location, void* value, unsigned int type,unsigned int count);
public:
	Shader(const char* vertexPath, const char* fragmentPath,const char* geomPath);
	//Should not use!
	Shader(const char* shaderPath);
	Shader();
	~Shader();
	void UseShader();

	unsigned int GetProgram();
	//Mem leak?
	void Reload();
	//Type should be GL_FLOAT_MAT4,GL_FLOAT_VEC3 and the like
	void UniformEquals(const char* uniform_Name, unsigned int type, void* value,unsigned int count);
	void ArrayUniformEquals(const char* uniformName, unsigned int type, void* value,unsigned int count);
	//Don't use!
	void UniformMatrix(std::string uniform_Name,glm::mat4* matrix,unsigned int type);
	//Don't use!
	void UniformVector(std::string uniform_Name, glm::vec3* vec);
};
extern Shader* defaultShader;