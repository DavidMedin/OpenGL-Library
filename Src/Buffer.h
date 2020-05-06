#pragma once
#include <stdio.h>
#include <string>
#include <tuple>

#include <glm.hpp>

#include "Error.h"
#include "GlCall.h"
#include "Shader.h"


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
	UniformBuffer(unsigned int types[][2], unsigned int typesNum, void** data, unsigned int bindingPoint,Shader* shads[],unsigned int shadNum, std::string uniformName);
	void Bind();
	void UnBind();
};

class StorageBuffer {
private:
	unsigned int bufferId;
	unsigned int bindingPoint;
	void* data;
	unsigned int size;

	int arrayType;
public:
	StorageBuffer();
	//last element type is assumed to be the variable length array type if it is an array (always include!)
	StorageBuffer(unsigned int types[][2],unsigned int typesNum, void** data, unsigned int bindingPoint, Shader* shads[], unsigned int shadNum, std::string storageName);
	void Bind();
	void UnBind();

	unsigned int GetSize();

	//returns pointer to write/read from
	void* MapData();
	//call this to free mapped data and write data to opengl (apply it)
	void UnMapData();
	//call this to resize, will copy data from old data to new data (will truncate if needed) - new data is initialized to zero
	//returns old size - use this to write to end of old data
	//unsigned int ResizeData(unsigned int newSize);

	//will rewrite the last element of the storage buffer IF IT IS AN ARRAY!!! (must be an unsized array in GLSL, check it)
	void AdjustVarElement(unsigned int newElementNum,void* data);
};

void* ToStd140(unsigned int types[][2],unsigned int typesNum, void** data,unsigned int* size);