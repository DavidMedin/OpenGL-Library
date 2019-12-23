#include "pch.h"
#include "Mesh.h"
#define GRAPHICSLIBRARY_EXPORTS 1
#include "Init.h"

Mesh::Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount) {
	/*transform = new DataTransform();
	((DataTransform*)transform)->matrix = new mat4(1.0);*/
	transform = new mat4(1.0f);
	vertecies = new VertexBuffer(data, size);
	VA = new VertexArray();
	VA->BindVertexBuffer(vertecies, 3, GL_FLOAT, false);
	vertexIndex = new IndexBuffer(indexData, indexCount);
	this->indexCount = indexCount;
}

Mesh::Mesh(string path)
{
	string warn;
	string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
	if (!warn.empty()) {
		NewError(warn);
	}
	if (!err.empty()) {
		NewError(err);
	}
	transform = new mat4(1.0f);
	vertecies = new VertexBuffer(attrib.vertices.data(), attrib.vertices.size()*sizeof(float));
	VA = new VertexArray();
	VA->BindVertexBuffer(vertecies, 3, GL_FLOAT, false);
	//might need more than one vertexIndex object for more than one shapes per mesh in the future
	//need to perform increment pointer arithetic in order to jump to next vertex index
	vertexIndex = new IndexBuffer((unsigned int *)&shapes.front().mesh.indices.front().vertex_index, shapes.front().mesh.indices.size()*3);
	//this->indexCount = indexCount;
	indexCount = shapes.front().mesh.indices.size() * 3;
}


void Mesh::Draw()
{
	if (defaultShader != nullptr) defaultShader->UseShader();
	else {
		NewError("Default Shader is Null, create new shader with Shader(const char* vertexPath, const char* fragmentPath,true)");
		return;
	}
	VA->Bind();
	vertexIndex->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
}
void Mesh::Draw(Shader* shad)
{
	shad->UseShader();
	VA->Bind();
	//vertexIndex->Bind();
	//GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}
