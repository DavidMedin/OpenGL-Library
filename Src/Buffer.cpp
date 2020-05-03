#include "Buffer.h"

int SizeofGlEnum(int type)
{
	int size;
	switch (type) {
	case GL_FLOAT_VEC2: size = sizeof(glm::vec2); break;
	case GL_FLOAT_VEC3: size = sizeof(glm::vec3); break;
	case GL_FLOAT_VEC4: size = sizeof(glm::vec4); break;
	case GL_FLOAT: size = sizeof(float); break;
	case GL_INT: size = sizeof(int); break;
	case GL_BOOL: size = sizeof(bool); break;
	case GL_FLOAT_MAT4: size = sizeof(glm::vec4); break; 
	default: printf("invalid input to 'SizeofGLEnum'!\n"); return NULL;//-----------------------------------
	}
	return size;
}
//use ONLY for std140 layout
int GetAlignmentOf(int type) {
	int N = 4; // 4 bytes
	switch (type) {
	case GL_FLOAT:
	case GL_INT:
	case GL_BOOL: return N;
	case GL_FLOAT_VEC2: return 2 * N;
	case GL_FLOAT_VEC3:
	case GL_FLOAT_VEC4: return 4 * N;
	case GL_FLOAT_MAT4: printf("handle matrices outside of this function!\n"); return 0;
	default: printf("type %d is not handled!", type); return 0;
	}
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

UniformBuffer::UniformBuffer(std::initializer_list<unsigned int[2]> types, void** data, unsigned int bindingPoint, Shader* shads[], unsigned int shadNum, std::string uniformName)
{
	GLCall(glGenBuffers(1, &bufferId));
	Bind();

	unsigned int size;
	void* newData = ToStd140(types, data,&size);
	

	GLCall(glBufferData(GL_UNIFORM_BUFFER, size, newData, GL_STATIC_DRAW));
	bindingPoint = nextBinding;
	nextBinding++;

	//bind the uniform blocks in shader code to a binding point
	for (unsigned int i = 0; i < shadNum;i++) {
		shads[i]->UseShader();
		unsigned int index = glGetUniformBlockIndex(shads[i]->GetProgram(), uniformName.c_str());
		GLCall(glUniformBlockBinding(shads[i]->GetProgram(), index, bindingPoint));
		GLCall(glUseProgram(0));

	}
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferId));
	this->data = (char*)newData;
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

void* ToStd140(std::initializer_list<unsigned int[2]> types, void** data, unsigned int* size)
{
	//finds the number of total elements
	unsigned int alignNum = 0;
	for (auto i : types) {
		if (i[0] == GL_FLOAT_MAT4) {
			alignNum += 4; //adds num of elements
		}
		else alignNum += i[1];
	}

	unsigned int _size = 0; //in bytes
	unsigned int* alignments = (unsigned int*)malloc(sizeof(unsigned int) * alignNum);
	unsigned int* offsets = (unsigned int*)malloc(sizeof(unsigned int) * alignNum);

	//default offset
	offsets[0] = 0;

	char* newData;

	int offset = 0;
	int inc = 0;
	for (int i = 0; (unsigned int)i < types.size(); i++) {
		const unsigned int* u = *(types.begin() + i);

			if (u[1] > 1) {
				//----------------------- later - impl matrix array
				//all elements are size of vec4
				_size += sizeof(glm::vec4) * u[1];
				//populate the alignments (per element)
				for (int i = 0; (unsigned int)i < u[1]; i++) {
					alignments[inc] = sizeof(glm::vec4);

					//ajust offset
					if (offset % alignments[inc] != 0) { //if the offset is incorrect
					//inc -1 to compensate for adding one to inc after writing
						unsigned int fix = (alignments[inc] - (offset % alignments[inc]));
						offset += alignments[inc] + fix;
						_size += fix;
						offsets[inc] = offset;
					}
					else {
						offsets[inc] = offset;
						offset += alignments[inc];
					}

					inc++;//next element of alignments
				}

			}
			else if (u[0] == GL_FLOAT_MAT4) {
				_size += sizeof(glm::mat4);
				printf("inc size %u -> %u\n", sizeof(glm::mat4), _size);

				//write to alignments
				for (int i = 0; i < 4; i++) {
					alignments[inc] = sizeof(glm::vec4);

					//ajust offset
					if (offset % alignments[inc] != 0) { //if the offset is incorrect
					//inc -1 to compensate for adding one to inc after writing
						unsigned int fix = (alignments[inc] - (offset % alignments[inc]));
						offset += alignments[inc] + fix;
						_size += fix;
						offsets[inc] = offset - alignments[inc];
					}
					else {
						offsets[inc] = offset;
						offset += alignments[inc];
					}

					inc++;
				}
			}
			else
			{
				_size += GetAlignmentOf(u[0]);
				alignments[inc] = GetAlignmentOf(u[0]);

				printf("inc size %u -> %u\n", GetAlignmentOf(u[0]), _size);

				//ajust offset
				if (offset % alignments[inc] != 0) { //if the offset is incorrect
					//inc -1 to compensate for adding one to inc after writing
					unsigned int fix = (alignments[inc] - (offset % alignments[inc]));
					offset += alignments[inc] + fix;
					_size += fix;
					offsets[inc] = offset - alignments[inc];
				}
				else {
					offsets[inc] = offset;
					offset += alignments[inc];
				}

				inc++;
			}		
	}

	//allocate and write to newData
	int lastTypeIndex = 0;
	unsigned int lastElementNum = ((unsigned int*)(types.begin() + lastTypeIndex))[1];
	int currentElement = 1;
	//int lastIndex=0;
	newData = (char*)malloc(_size);
	for (int i = 0; (unsigned int)i < alignNum; i++) {
		int typeSize = SizeofGlEnum(*(types.begin() + lastTypeIndex)[0]);
		int byteOffsetElement = (currentElement - 1) * typeSize;//array element * typeSize
		if (offsets[i] + typeSize > _size) {
			printf("Whaooo!");
		}
		memcpy(&newData[offsets[i]], &(((char*)data[lastTypeIndex])[byteOffsetElement]), typeSize);

		if (currentElement == lastElementNum) { //reached the end of the type
			lastTypeIndex++; //inc type pointer
			if (((unsigned int*)(types.begin() + lastTypeIndex))[0] == GL_FLOAT_MAT4) {
				lastElementNum = 4;
			}
			else
				lastElementNum = ((unsigned int*)(types.begin() + lastTypeIndex))[1];
			currentElement = 0;
		}
		currentElement++;
	}
	free(offsets);
	free(alignments);
	*size = _size;
	return newData;
}

unsigned int StorageBufferBind = 0;
StorageBuffer::StorageBuffer()
{
}

StorageBuffer::StorageBuffer(std::initializer_list<unsigned int[2]> types, void** data, unsigned int bindingPoint, Shader* shads[], unsigned int shadNum, std::string storageName)
{
	//converts data to std140
	unsigned int size;
	void* newData = ToStd140(types, data, &size);

	GLCall(glGenBuffers(1, &bufferId));
	Bind();

	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, newData, GL_STATIC_DRAW));
	bindingPoint = StorageBufferBind;
	StorageBufferBind++;
	
	//bind the uniform blocks in shader code to a binding point
	for (unsigned int i = 0; i < shadNum; i++) {
		shads[i]->UseShader();
		unsigned int index = glGetProgramResourceIndex(shads[i]->GetProgram(), GL_SHADER_STORAGE_BLOCK, storageName.c_str());
		if (index == GL_INVALID_INDEX) {
			printf("%s is not a valid shader storage block in your shader!\n", storageName.c_str());
		}
		GLCall(glShaderStorageBlockBinding(shads[i]->GetProgram(), index, bindingPoint));
		GLCall(glUseProgram(0));

	}
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, bufferId));
	this->data = (char*)newData;
	UnBind();
}

void StorageBuffer::Bind()
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferId));
}

void StorageBuffer::UnBind()
{
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}
