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
using namespace glm;

 
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

class Transform /*: Attribute*/ {
private:

public:
	mat4* data;

	Transform();
	Transform(mat4* transform);

};


class Texture {
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

 void DrawFlags(int flag/*toggle*/);
 bool GetDrawFlags(int flag/*toggle*/);

class Mesh {
private:
	VertexBuffer* vertexBuffer;
	VertexBuffer* textureUVBuffer;
	VertexBuffer* normalBuffer;
	//inc mandNum whenever a built-in buffer is added! (in VertexBuffer.h)
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
	//untested!!!
	void BindCustomData(VertexBuffer* data, unsigned int type, unsigned int vecX); //untested
	void Bind();
};

class Line {
private:
	VertexBuffer* VB;
	VertexArray* VA;
	//xyz,xyz - ONLY TWO VERTECIES (6 floats)
	float* points;
	float* mappedPoints;
public:
	vec3 color;
	float size;
	Line();
	Line(vec3 point1, vec3 point2);
	Line(vec3 point1, vec3 point2,vec3 color);
	void SetPoint1(vec3 point);
	void SetPoint2(vec3 point);
	vec3 GetPoint1();
	vec3 GetPoint2();
	void Draw(Camera* cam);
	void Draw(Shader* shad,Camera* cam);
	float* OpenWriting();
	unsigned int ClosedWriting();
};