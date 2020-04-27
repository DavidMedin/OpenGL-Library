#include "Buffer.h"

int SizeofGlEnum(int type)
{
	int size;
	switch (type) {
	case GL_FLOAT_VEC2: size = sizeof(glm::vec2); break;
	case GL_FLOAT_VEC3: size = sizeof(glm::vec3); break;
	case GL_FLOAT: size = sizeof(float); break;
	case GL_INT: size = sizeof(int); break;
	case GL_BOOL: size = sizeof(bool); break;
	case GL_FLOAT_MAT4: size = sizeof(glm::mat4); break;
	default: printf("invalid input to 'SizeofGLEnum'!\n"); return NULL;
	}
	return size;
}



IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) {

	m_Count = count;
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
//don't use
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, const unsigned int offset) {
	m_Count = count;
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(int), data));
}
IndexBuffer::IndexBuffer() {
	m_Count = NULL;
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

}
IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
void IndexBuffer::Unbind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}




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
	mappedData = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	return mappedData;
}

unsigned int VertexBuffer::UnmapData()
{
	Bind();
	unsigned int ret = glUnmapBuffer(GL_ARRAY_BUFFER);
	return ret;
}


VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
	Bind();
	nextIndex = 0;
	nextCustomIndex = 0;
}
void VertexArray::Bind() {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::BindVertexBuffer(VertexBuffer* buffer, unsigned int vecX, unsigned int type, bool normalize) {
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
	GLCall(glEnableVertexAttribArray(nextCustomIndex + MANDNUM));
	GLCall(glVertexAttribPointer(nextCustomIndex + MANDNUM, vecX, type, normalize, 0, NULL));
	nextCustomIndex++;
}


unsigned int nextBinding=0;

UniformBuffer::UniformBuffer()
{
	bufferId = NULL;
}

UniformBuffer::UniformBuffer(std::initializer_list<unsigned int[2]> types,void* data, unsigned int bindingPoint,bool toStd140, Shader* shads[], unsigned int shadNum, std::string uniformName)
{
	GLCall(glGenBuffers(1, &bufferId));
	Bind();
	
	unsigned int size=0;
	unsigned int* alignments = (unsigned int*)malloc(sizeof(unsigned int) * types.size());
	//using char pointer for one byte offsets
	char* newData = nullptr;

	std::list<unsigned int[2]> newTypes;
	for (int i = 0; i < types.size();i++) {
		unsigned int* u = (unsigned int*)(types.begin() + i);
		if (toStd140) {
			int N = 4;
			switch (u[0]) {
			case GL_BOOL:{}
			case GL_FLOAT:{}
			case GL_INT: {
				//if it is a array, each slot has a size of 4N
				alignments[i] = (u[1] == 1 ? N : 4 * N * u[1]);
				size += alignments[i];
				break;
			}
			case GL_FLOAT_VEC2: {
				alignments[i] = u[1] == 1 ? 2 * N : (2*N)*u[1];
				size += alignments[i];
				break;
			}
			case GL_FLOAT_VEC3: {}
			case GL_FLOAT_VEC4: {
				alignments[i] = (4 * N)*u[1];
				size += alignments[i];
				break;
			}
			case GL_FLOAT_MAT4: {
				//is stored like an array of 4 vec4s. or, an array of #column vec#Rows
				//alignments[inc] = (4 * N) * 4 * i[1];
				//size += alignments[inc];
				break;
			}
			}
		}else size += SizeofGlEnum(u[0])*u[1]; // should not take size of a number!!!
	}
	if (toStd140) {
		unsigned int currentOffset=0; // where the writing pointer is
		unsigned int dataOffset = 0;
		newData = (char*)malloc(size);
		if (newData != nullptr) {
			for (int i = 0; i < types.size(); i++) {
				//brings currentoffset to a multiple of the size of the element
				if (currentOffset % alignments[i] != 0) currentOffset += alignments[i] - (currentOffset % alignments[i]); //jump to next multiple of alignments[i]

				unsigned int tmpOffset = 0;

				//copied memcpy in like this because it is incompatible with mat3 or mat3x4 or whatever
				std::cout << "num " << i + 1 << " alignment: " << alignments[i] << "\n";
				switch (types.begin()[i][0]) {
				case GL_INT: {
					tmpOffset += sizeof(int) * types.begin()[i][1];
					memcpy(&newData[currentOffset], &((char*)data)[dataOffset], tmpOffset);
					break;}

				case GL_FLOAT: {
					tmpOffset += sizeof(float) * types.begin()[i][1];
					memcpy(&newData[currentOffset], &((char*)data)[dataOffset], tmpOffset);
					break; }
				case GL_BOOL: {
					tmpOffset += sizeof(bool) * types.begin()[i][1];
					memcpy(&newData[currentOffset], &((char*)data)[dataOffset], tmpOffset);
					break; }
				case GL_FLOAT_VEC2: {
					tmpOffset += sizeof(glm::vec2) * types.begin()[i][1];
					memcpy(&newData[currentOffset], &((char*)data)[dataOffset], tmpOffset);
					break; }

				case GL_FLOAT_VEC3: {
					tmpOffset += sizeof(glm::vec3) * types.begin()[i][1];
					memcpy(&newData[currentOffset], &((char*)data)[dataOffset], tmpOffset);
					break; }

				case GL_FLOAT_MAT4: {
					tmpOffset += sizeof(glm::mat4) * types.begin()[i][1];
					//still need to deal with
					memcpy(&newData[currentOffset], &((char*)data)[dataOffset], tmpOffset);
					break; }
				default: printf("type of %u is not an available element of a uniform buffer!\n", types.begin()[i][0]);
				}
				dataOffset += tmpOffset;
				currentOffset += tmpOffset;
			}
		}
	}


	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, toStd140 ? newData : data, GL_STATIC_DRAW));
	bindingPoint = nextBinding;
	nextBinding++;

	//bind the uniform blocks in shader code to a binding point
	for (unsigned int i = 0; i < shadNum;i++) {
		shads[i]->UseShader();
		unsigned int index = glGetUniformBlockIndex(shads[i]->GetProgram(), uniformName.c_str());
		GLCall(glUniformBlockBinding(shads[i]->GetProgram(), index, bindingPoint));
	}
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferId));
	free(alignments);
	data = newData != nullptr ? newData : data;
	UnBind();
}



void UniformBuffer::Bind()
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, bufferId));
}

void UniformBuffer::UnBind()
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER,0));
}
