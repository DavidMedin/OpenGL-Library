#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>
//#include "Maths.h"

#include "Export.h"
#include "GLCall.h"
#define TYPE_NULL 0
#define TYPE_PROJECTION 1
#define TYPE_MODEL 2
#define TYPE_VIEW 3

#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56

#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C

#define GL_SAMPLER_2D 0x8B5E

#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
namespace Cedar {
	class GL_EXPORT Shader {
	private:
		std::string vertexContent;
		std::string fragmentContent;
		std::string geometryContent; //might fail

		//SPIRV-VM -- for debugging
		class ImplSpvShader;
		ImplSpvShader* spvShaders[2];
		unsigned int spvShaderNum = 2;
		bool spvInitialized = false;
		//you must delete when you're done with it btw
		std::list<ImplSpvShader*>* SPIRVVMFindUniform(std::string name);

		unsigned int shader_Program;

		std::string ReadShader(const char* path);
		unsigned int CompileShader(unsigned int type, const char* source);
		unsigned int CreateShaderProgram(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
		void _UniformEquals(int location, void* value, unsigned int type, unsigned int count, const char* uniformName);
	public:
		Shader(const char* vertexPath, const char* fragmentPath, const char* geomPath);
		//Should not use!
		Shader(const char* shaderPath);
		Shader();
		~Shader();
		void UseShader();

		unsigned int GetProgram();
		//Mem leak?
		void Reload();
		//Type should be GL_FLOAT_MAT4,GL_FLOAT_VEC3 and the like
		void UniformEquals(const char* uniform_Name, unsigned int type, void* value, unsigned int count);
		void ArrayUniformEquals(const char* uniformName, unsigned int type, void* value, unsigned int count);
		void BindInterfaceBlock(std::string name, unsigned int interfaceType, unsigned int bindingPoint);

		//-- spriv-vm debugging
		bool GetSPIRVVMInitialized();
		void StartSPIRVVMDebug();
		void InitializeSPIRVVMDebug();
		//not finished, spirvvm needs std140 first
		void SPIRVVMInterfaceWrite(std::string blockName, unsigned int type, unsigned int localIndex, void* data, unsigned int primitiveType, unsigned int sizeofData);
	};
	//extern Shader* defaultShader;
}