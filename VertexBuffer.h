#pragma once
#define _TRI unsigned 3
#include <stdio.h>
//temp
#include "Error.h"
#include <string>
class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();
	void Modify(const void* data, unsigned int size);
	void Bind();
	void Unbind();
};

class VertexArray {
private:
	unsigned int m_RendererID;
	unsigned int nextIndex;
public:
	void Bind();
	VertexArray();
	void BindVertexBuffer(VertexBuffer* buffer, unsigned int vecX, unsigned int type, bool normalize);
};