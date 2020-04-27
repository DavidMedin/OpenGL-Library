#pragma once
#include <stdio.h>
#include <string>
#include <tuple>

#include <glm.hpp>

#include "Error.h"
#include "GlCall.h"
#include "Shader.h"

#define STD140 1

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(const unsigned int* data, unsigned int count, const unsigned int offset);
	IndexBuffer();
	~IndexBuffer();

	void Bind();
	void Unbind();
};

#define _TRI unsigned 3
#define MANDNUM 4
class VertexBuffer {
private:
	unsigned int m_RendererID;
	void* mappedData;
public:
	VertexBuffer(void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();
	void Modify(const void* data, unsigned int size);
	void Bind();
	void Unbind();
	void* MapData();
	unsigned int UnmapData();
};

class VertexArray {
private:
	unsigned int m_RendererID;
	unsigned int nextIndex;//mand index
	unsigned int nextCustomIndex;
public:
	void Bind();
	VertexArray();
	void BindVertexBuffer(VertexBuffer* buffer, unsigned int vecX, unsigned int type, bool normalize);
	void BindIntVertexBuffer(VertexBuffer* buffer);
	void BindCustomBuffer(VertexBuffer* buffer, unsigned int vecX, unsigned int type, bool normalize);
};

class FrameBuffer {
private:
	unsigned int m_RendererID;
	unsigned int nextIndex;
public:
	FrameBuffer();


};

//one uniform buffer, one binding point, one uniform block
class UniformBuffer {
private:
	unsigned int bufferId;
	unsigned int bindingPoint;
	char* data;
public:
	UniformBuffer();
	//types = {{GL_FLOAT_MAT4,2},...}
	//must be according to the layout specifier of the uniform buffer defined in your shader
	UniformBuffer(std::initializer_list<unsigned int[2]> types, void* data, unsigned int bindingPoint,bool toStd140,Shader* shads[],unsigned int shadNum, std::string uniformName);
	void Bind();
	void UnBind();
};