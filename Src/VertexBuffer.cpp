 
#include "VertexBuffer.h"	
#include "GLCall.h"

VertexBuffer::VertexBuffer(void* data, unsigned int size) {
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}
VertexBuffer::VertexBuffer() {
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW));
}
VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Modify(const void* data, unsigned int size) {
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

void VertexBuffer::Bind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::Unbind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void* VertexBuffer::MapData()
{
	Bind();
	mappedData = glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
	return mappedData;
}

unsigned int VertexBuffer::UnmapData()
{
	Bind();
	unsigned int ret = glUnmapBuffer(GL_ARRAY_BUFFER);
	return ret;
}


VertexArray::VertexArray(){
	GLCall(glGenVertexArrays(1, &m_RendererID));
	Bind();
	nextIndex = 0;
	nextCustomIndex = 0;
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

void VertexArray::BindIntVertexBuffer(VertexBuffer* buffer)
{
	buffer->Bind();
	Bind();
	GLCall(glEnableVertexAttribArray(nextIndex));
	GLCall(glVertexAttribIPointer(nextIndex, 1, GL_INT, 0, NULL));
	nextIndex++;
}

void VertexArray::BindCustomBuffer(VertexBuffer* buffer, unsigned int vecX, unsigned int type, bool normalize)
{
	buffer->Bind();
	Bind();
	GLCall(glEnableVertexAttribArray(nextCustomIndex+MANDNUM));
	GLCall(glVertexAttribPointer(nextCustomIndex + MANDNUM, vecX, type, normalize, 0, NULL));
	nextCustomIndex++;
}
