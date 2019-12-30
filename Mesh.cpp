#include "pch.h"
#include "Mesh.h"
#define GRAPHICSLIBRARY_EXPORTS 1
#include "Init.h"

Mesh::Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount) {
	/*transform = new DataTransform();
	((DataTransform*)transform)->matrix = new mat4(1.0);*/
	transform = new mat4(1.0f);
	vertices = new VertexBuffer(data, size);
	VA = new VertexArray();
	VA->BindVertexBuffer(vertices, 3, GL_FLOAT, false);
	vertexIndex = new IndexBuffer(indexData, indexCount);
	this->indexCount = indexCount;
}

Mesh::Mesh(string path)
{
	Assimp::Importer importer;
	//aiProcess_JoinIdenticalVertices
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (scene == NULL) {
		NewError(importer.GetErrorString());
		return;
	}
	printf("number of Meshes = :%d\n", scene->mNumMeshes);
	const aiMesh* mesh = scene->mMeshes[0];
	unsigned int point_count = mesh->mNumVertices;
	float* points = nullptr;
	if (mesh->HasPositions()) {
		points = (float*)malloc(sizeof(float) * point_count*3);
		if (points != nullptr && point_count != 0) {
			for (unsigned int i = 0; i < point_count; i++) {
				aiVector3D vector = mesh->mVertices[i];
				points[i * 3] = (float)vector.x;
				points[i * 3 + 1] = (float)vector.y;
				points[i * 3 + 2] = (float)vector.z;
			}
		}
		else {
			NewError("Could not allocate memory for (GLFloat* points) in Mesh.cpp\n");
			return;
		}
	}
	transform = new mat4(1.0f);
	vertices = new VertexBuffer(points,sizeof(float)*point_count*3);
	pointCount = point_count;
	VA = new VertexArray();
	VA->BindVertexBuffer(vertices, 3, GL_FLOAT, false);
	//free(points);
	//index buffer
	unsigned int* indices = nullptr;
	unsigned int face_count = mesh->mNumFaces;
	if (mesh->HasFaces()) {
		indices = (unsigned int*)malloc(sizeof(unsigned int) * face_count * 3);
		if (indices != nullptr && face_count != 0) {
			for (unsigned int i = 0; i < face_count; i++) {
				aiFace face = mesh->mFaces[i];
				if (face.mNumIndices == 3) {
					indices[i * 3] = (unsigned int)face.mIndices[0];
					indices[i * 3 + 1] = (unsigned int)face.mIndices[1];
					indices[i * 3 + 2] = (unsigned int)face.mIndices[2];
				}
				else {
					NewError("A face had more than 3 points, is 'aiProcess_Triangulate' not flagged?\n");
				}
			}
		}
	}
	vertexIndex = new IndexBuffer(indices,face_count*3);
	this->indexCount = face_count*3;
	//free(indices);
	
	//normals
	//use point_count for size
	float* normals = nullptr;
	if (mesh->HasNormals()) {
		normals = (float*)malloc(sizeof(float) * 3 * point_count);
		if (normals != nullptr) {
			for (unsigned int i = 0; i < point_count; i++) {
				aiVector3D vector = mesh->mNormals[i];
				normals[i * 3] = (float)vector.x;
				normals[i * 3 + 1] = (float)vector.y;
				normals[i * 3 + 2] = (float)vector.z;
			}
		}
		else {
			NewError("the array normals in Mesh.cpp didn't get memory allocated\n");
			return;
		}
	}
	normalBuffer = new VertexBuffer(normals, point_count*3 * sizeof(float));
	VA->BindVertexBuffer(normalBuffer, 3, GL_FLOAT, false);
	importer.FreeScene();
	printf("Number of Points :%d\n", pointCount);
	NewError(importer.GetErrorString());

	//printf("Number of Indices :%d\n", indexCount);
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
	GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT,nullptr));
	//GLCall(glDrawArrays(GL_TRIANGLES, 0, pointCount));
}
void Mesh::Draw(Shader* shad)
{
	shad->UseShader();
	VA->Bind();
	//vertexIndex->Bind();
	//GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}
