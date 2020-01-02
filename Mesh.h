#pragma once
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
//assimp
#include <assImp/Importer.hpp>
#include <assImp/scene.h>
#include <assimp/postprocess.h>

//stb
#include <stb_image.h>

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

class GRAPHICSLIBRARY_API Texture {
private:
	unsigned int openglID;
	unsigned int height, width;
	float* data;
public:
	Texture(unsigned int slot, float* data, unsigned int w, unsigned int h);
	Texture();
	void Bind(unsigned int slot = 0);
};


class GRAPHICSLIBRARY_API Mesh {
private:
	unsigned int indexCount; //called 'elements'
	unsigned int pointCount;
	VertexBuffer* vertexBuffer;
	VertexBuffer* textureUVBuffer;
	VertexBuffer* normalBuffer;
	VertexArray* VA;
	IndexBuffer* index;
	mat4* transform;

	Texture texList[32];

	float* vertices;
	float* normals;
	float* textureUVs;
	unsigned int* indices;
public:
	//Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount);
	Mesh(string path);
	void Draw();
	void Draw(Shader* shad);
};

