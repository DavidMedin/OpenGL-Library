#pragma once
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
//assimp
#include <assImp/Importer.hpp>
#include <assImp/scene.h>
#include <assimp/postprocess.h>
//glm
#include <glm.hpp>
#include <ext.hpp>
//glew
#include <glew.h>
using namespace glm;

#else
#define GRAPHICSLIBRARY_API __declspec(dllimport)
#endif
#include "Maths.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Error.h"
#include <vector>




class GRAPHICSLIBRARY_API Mesh {
private:
	unsigned int indexCount; //called 'elements'
	unsigned int pointCount;
	VertexBuffer* vertices;
	//VertexBuffer* textureUV;
	VertexBuffer* normalBuffer;
	VertexArray* VA;
	IndexBuffer* vertexIndex;
	mat4* transform;

	
public:
	Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount);
	Mesh(string path);
	void Draw();
	void Draw(Shader* shad);
};

