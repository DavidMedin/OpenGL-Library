#pragma once



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