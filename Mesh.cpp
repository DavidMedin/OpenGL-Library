#include "pch.h"
#include "Mesh.h"
#define GRAPHICSLIBRARY_EXPORTS 1
#include "Init.h"


//-------------InterfaceMesh-----------------------------------//
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
