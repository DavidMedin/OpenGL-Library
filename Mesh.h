#pragma once
#ifdef GRAPHICSLIBRARY_EXPORTS
#define GRAPHICSLIBRARY_API __declspec(dllexport)
#include <glm.hpp>
#include <ext.hpp>
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




class GRAPHICSLIBRARY_API Mesh {
private:
	unsigned int indexCount; //called 'elements'
	VertexBuffer* vertecies;
	VertexArray* VA;
	IndexBuffer* vertexIndex;
	mat4* transform;
public:
	Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount);
	void Draw();
	void Draw(Shader* shad);
};

