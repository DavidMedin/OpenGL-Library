#pragma once
#define _TRI unsigned 3
#include <stdio.h>
//temp
#include "Error.h"
#include <string>

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