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

//used for debugging, one per shader, not program
struct spvShader {
	spvm_context_t spvContext;
	unsigned int spvSLength;
	spvm_source spvSource;
	//--
	spvm_program_t spvProgram;
	spvm_state_t spvState;
	//shaderType is 
	spvShader();
	spvShader(std::string content, unsigned int shaderType);
};

class Shader {
private:
	std::string vertexContent;
	std::string fragmentContent;
	std::string geometryContent; //might fail
	//list<string> inputTypes;

	//SPIRV-VM -- for debugging
	spvShader spvVertex;
	spvShader spvFragment;
	bool spvInitialized=false;

	unsigned int shader_Program;

	std::string ReadShader(const char* path);
	unsigned int CompileShader(unsigned int type, const char* source);
	unsigned int CreateShaderProgram(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
	void _UniformEquals(int location, void* value, unsigned int type,unsigned int count,const char* uniformName,unsigned int shaderType);
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
	void UniformEquals(const char* uniform_Name, unsigned int type, void* value,unsigned int count, unsigned int shaderType);
	void ArrayUniformEquals(const char* uniformName, unsigned int type, void* value,unsigned int count, unsigned int shaderType);
	void BindInterfaceBlock(std::string name, unsigned int interfaceType, unsigned int bindingPoint);

	//-- spriv-vm debugging
	bool GetSPIRVVMInitializer();
	void StartSPIRVVMDebug();
	void InitializeSPIRVVMDebug();
	void SPIRVVMInterfaceWrite();
};
//extern Shader* defaultShader;