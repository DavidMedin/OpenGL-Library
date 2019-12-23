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
#define TINYOBJLOADER_IMPLEMENTATION

#include "Dependancies/Tiny-Obj-Loader/tiny_obj_loader.h"



class GRAPHICSLIBRARY_API Mesh {
private:
	unsigned int indexCount; //called 'elements'
	VertexBuffer* vertecies;
	VertexArray* VA;
	IndexBuffer* vertexIndex;
	mat4* transform;

	//Tiny Obj Loader
	tinyobj::attrib_t attrib; // contains all of the data (vertices, normals, uv etc.)
	vector<tinyobj::shape_t> shapes; // data type of index references to attrib
	vector<tinyobj::material_t> materials; // apperance data
public:
	Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount);
	Mesh(string path);
	void Draw();
	void Draw(Shader* shad);
};

