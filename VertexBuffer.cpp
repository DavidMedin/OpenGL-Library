#include "pch.h"
#include "VertexBuffer.h"	
#include "GLCall.h"

VertexBuffer::VertexBuffer(float* data, unsigned int size) {
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Modify(const void* data, unsigned int size) {
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::Unbind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


VertexArray::VertexArray(){
	GLCall(glGenVertexArrays(1, &m_RendererID));
	Bind();
	nextIndex = 0;
}
void VertexArray::Bind() {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::BindVertexBuffer(VertexBuffer* buffer, unsigned int vecX, unsigned int type,bool normalize) {
	buffer->Bind();
	Bind();
	GLCall(glEnableVertexAttribArray(nextIndex));
	GLCall(glVertexAttribPointer(nextIndex, vecX, type, normalize, 0, NULL));
	nextIndex++;
}