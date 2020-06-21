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
 
//SPRIV-VM
#include <spvm/context.h>
#include <spvm/state.h>
#include <spvm/ext/GLSL450.h>

//shaderc
#include <shaderc/shaderc.hpp>

#define TYPE_NULL 0
#define TYPE_PROJECTION 1
#define TYPE_MODEL 2
#define TYPE_VIEW 3

#include <memory>
class Shader {
private:
	std::string vertexContent;
	std::string fragmentContent;
	std::string geometryContent; //might fail

	//SPIRV-VM -- for debugging
	class ImplSpvShader;
	ImplSpvShader* spvShaders[2];
	unsigned int spvShaderNum = 2;
	bool spvInitialized=false;
	//you must delete when you're done with it btw
	std::list<ImplSpvShader*>* SPIRVVMFindUniform(std::string name);

	unsigned int shader_Program;

	std::string ReadShader(const char* path);
	unsigned int CompileShader(unsigned int type, const char* source);
	unsigned int CreateShaderProgram(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
	void _UniformEquals(int location, void* value, unsigned int type,unsigned int count,const char* uniformName);
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
	void BindInterfaceBlock(std::string name, unsigned int interfaceType, unsigned int bindingPoint);

	//-- spriv-vm debugging
	bool GetSPIRVVMInitialized();
	void StartSPIRVVMDebug();
	void InitializeSPIRVVMDebug();
	//not finished, spirvvm needs std140 first
	void SPIRVVMInterfaceWrite(std::string blockName,unsigned int type,unsigned int localIndex,void* data,unsigned int primitiveType, unsigned int sizeofData);
};
//extern Shader* defaultShader;