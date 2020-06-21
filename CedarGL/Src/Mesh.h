#pragma once
 
//assimp
#include <assImp/Importer.hpp>
#include <assImp/scene.h>
#include <assimp/postprocess.h>


//glm
#include <glm.hpp>
#include <ext.hpp>
//glew
#include <GL/glew.h>

 
#include "Maths.h"
#include "Init.h"
#include "Buffer.h"
#include "Shader.h"
#include "Error.h"
#include "Texture.h"
#include "Camera.h"
#include <vector>
#include "Bone.h"


class Mesh {
private:
	//for opengl
	VertexBuffer* vertexBuffer;
	VertexBuffer* textureUVBuffer;
	VertexBuffer* normalBuffer;
	VertexBuffer* boneIdBuffer; //contains a bone index for each vertex
	VertexBuffer* weightBuffer;

	VertexArray* VA;
	IndexBuffer* index;

	//for us
	float* vertices;
	float* normals;
	float* textureUVs;
	unsigned int* indices;
	int* boneIds;
	float* weights;
public:
	//you can change these and nothing bad will happen
	Skeleton* skelly;
	Texture* textures[32];

	unsigned int boneCount;
	unsigned int indexCount; //called 'elements'
	unsigned int pointCount;

	//use this for primitives
	Mesh(float* data, unsigned int size, unsigned int* indexData, unsigned int indexCount);

	//main mesh constructor
	Mesh(std::string path);
	int drawMode; //must be GL_TRIANGLES,GL_POINTS, or the like
	void Draw(Shader* shad,Camera* cam);
};