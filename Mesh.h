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

class GRAPHICSLIBRARY_API Attribute {
public:
	unsigned int type;
	Attribute(unsigned int type);
	Attribute();
};


class GRAPHICSLIBRARY_API Texture : Attribute {
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

//needs to belong to a sceneNode, or a class or struct that contains a "list<void*> attributeList;" in it
class GRAPHICSLIBRARY_API Mesh : Attribute {
private:
	VertexBuffer* vertexBuffer;
	VertexBuffer* textureUVBuffer;
	VertexBuffer* normalBuffer;
	VertexArray* VA;
	IndexBuffer* index;
	mat4* transform;

	//shouldn't belong to Mesh, rather be somewhere in the AttributeList
	Texture* texList[32];

	float* vertices;
	float* normals;
	float* textureUVs;
	unsigned int* indices;



public:
	unsigned int indexCount; //called 'elements'
	unsigned int pointCount;
	//Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount);
	Mesh(string path);
	//must be a pointer to a aiMesh!!
	//not recommended for the user of the Library unless you include AssiImp
	Mesh(void* mesh);
	void Draw(Camera* cam);
	void Draw(Shader* shad,Camera* cam);

	void Bind();
};

