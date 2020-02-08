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
#include "Camera.h"
#define DIFFUSE_SLOT 0
#define SPECULAR_SLOT 1

#define ATTRIBUTE_MESH 10
#define ATTRIBUTE_TEXTURE 11
#define ATTRIBUTE_TRANSFORM 12

#define DRAWFLAG_TRIANGLE 1
//00000000
//       ^-draw triangles; else points

class GRAPHICSLIBRARY_API Transform /*: Attribute*/ {
private:

public:
	mat4* data;

	Transform();
	Transform(mat4* transform);

};


class GRAPHICSLIBRARY_API Texture {
private:
	unsigned int openglID;
	unsigned int height, width;
	unsigned char* data;
	unsigned int slot;
public:
	Texture(unsigned int slot,unsigned char* data, unsigned int w, unsigned int h);
	Texture(unsigned int slot, std::string path);
	Texture();
	~Texture();
	void Bind(unsigned int slot = NULL);
};

GRAPHICSLIBRARY_API void DrawFlags(int flag/*toggle*/);
GRAPHICSLIBRARY_API bool GetDrawFlags(int flag/*toggle*/);

class GRAPHICSLIBRARY_API Mesh {
private:
	VertexBuffer* vertexBuffer;
	VertexBuffer* textureUVBuffer;
	VertexBuffer* normalBuffer;
	VertexArray* VA;
	IndexBuffer* index;
	mat4* transform;


	float* vertices;
	float* normals;
	float* textureUVs;
	unsigned int* indices;



public:
	//A reference to the textures, should also be in attributes
	Texture* texList[32];


	unsigned int indexCount; //called 'elements'
	unsigned int pointCount;
	//Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount);
	Mesh(string path);
	//must be a pointer to a aiMesh!!
	//not recommended for the user of the Library unless you include AssiImp
	Mesh(void* mesh);
	void Draw(Camera* cam);
	void Draw(Shader* shad,Camera* cam);

	void BindCustomData(VertexBuffer* data, unsigned int slot);
	void Bind();
};

