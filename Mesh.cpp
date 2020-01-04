#include "pch.h"
#include "Mesh.h"
#define GRAPHICSLIBRARY_EXPORTS 1
#include "Init.h"


Texture::Texture(unsigned int slot,unsigned char* data, unsigned int w, unsigned int h) {
	openglID = NULL;
	GLCall(glGenTextures(1, &openglID));
	this->slot = slot;
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	this->data = data;
	height = h;
	width = w;
}

Texture::Texture(unsigned int slot, std::string path) {
	stbi_set_flip_vertically_on_load(true);
	int x, y, n;
	unsigned char* data = stbi_load(path.c_str(), &x, &y, &n, STBI_rgb_alpha);
	if (data == nullptr) {
		printf("Path to %s not found!\n", path.c_str());
	}
	this->data = data;
	height = y;
	width = x;
	openglID = NULL;
	GLCall(glGenTextures(1, &openglID));
	this->slot = slot;
	Bind(slot);
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
}

Texture::Texture() {
	openglID = NULL;
	data = nullptr;
	height = NULL;
	width = NULL;
}
void Texture::Bind(unsigned int slot) {
	//will need a system for slot management for any given object
	GLCall(glBindTexture(GL_TEXTURE_2D, openglID));
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));

	//openglID = NULL;
	//data = nullptr;
	//height = NULL;
	//width = NULL;
}
Texture::~Texture() {
	stbi_image_free(this->data);
}


//Mesh::Mesh(float* data, unsigned int size, const unsigned int* indexData, unsigned int indexCount) {
//	/*transform = new DataTransform();
//	((DataTransform*)transform)->matrix = new mat4(1.0);*/
//	transform = new mat4(1.0f);
//	vertexBuffer = new VertexBuffer(data, size);
//	VA = new VertexArray();
//	VA->BindVertexBuffer(vertexBuffer, 3, GL_FLOAT, false);
//	index = new IndexBuffer(indexData, indexCount);
//	this->indexCount = indexCount;
//}

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
	vertexBuffer = new VertexBuffer(points,sizeof(float)*point_count*3);
	pointCount = point_count;
	VA = new VertexArray();
	VA->BindVertexBuffer(vertexBuffer, 3, GL_FLOAT, false);
	vertices = points;


	//index buffer
	unsigned int* indices = nullptr;
	unsigned int face_count = mesh->mNumFaces;
	if (mesh->HasFaces() && face_count >= 0) {
		indices = (unsigned int*)malloc(sizeof(unsigned int) * face_count * 3);
		if (indices != NULL) {
			for (int i = 0; i < (int)face_count; i++) {
				aiFace face = mesh->mFaces[i];
				if (face.mNumIndices == 3) {
					indices[i * 3] = face.mIndices[0];
					indices[i * 3 + 1] = face.mIndices[1];
					indices[i * 3 + 2] = face.mIndices[2];
				}
				else {
					NewError("A face had more than 3 points, is 'aiProcess_Triangulate' not flagged?\n");
				}
			}
		}
	}
	index = new IndexBuffer(indices,face_count*3);
	this->indexCount = face_count*3;
	this->indices = indices;
	
	//normals
	//use point_count for size
	float* normals = nullptr;
	if (mesh->HasNormals() && point_count >= 0) {
		normals = (float*)malloc(sizeof(float) * 3 * point_count);
		if (normals != NULL) {
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
	this->normals = normals;

	//texture UVs
	float* UVs = nullptr;
	if (mesh->HasTextureCoords(0) && point_count >= 0) {
		UVs = (float*)malloc(sizeof(float) * 2 * point_count);
		if (UVs != NULL) {
			for (int i = 0; i < point_count; i++) {
				UVs[i * 2 + 0] = mesh->mTextureCoords[0][i].x;
				UVs[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
			}
		}
		else {
			NewError("UVs' memory was incorrectly allocated\n");
		}
	}
	textureUVBuffer = new VertexBuffer(UVs, sizeof(float) * point_count * 2);
	VA->BindVertexBuffer(textureUVBuffer, 2, GL_FLOAT, false);
	this->textureUVs = UVs;


	//texture loading
	unsigned int num_textures = scene->mNumTextures;
	unsigned int num_material = scene->mNumMaterials;
	aiMaterial* material= nullptr;
	Texture* diffuseTexture;
	aiString textureName;

	size_t lastChar = path.find_last_of("/\\")+1;
	string pathTo = path.erase(lastChar);

	for (int i = 0; i < 32; i++) {
		texList[i] = nullptr;
	}


	if (scene->HasMaterials()) {
		material = scene->mMaterials[mesh->mMaterialIndex];

		unsigned int diffuseNum = material->GetTextureCount(aiTextureType_DIFFUSE);
		printf("Diffuse Textures : %d\n", diffuseNum);
		for (int i = 0; i < diffuseNum; i++) {
			if (diffuseNum == 1) {
			material->GetTexture(aiTextureType_DIFFUSE, i, &textureName);
			printf("	|-> %s\n", textureName.C_Str());
			diffuseTexture = new Texture(DIFFUSE_SLOT,pathTo + string(textureName.C_Str()));
			texList[DIFFUSE_SLOT] = diffuseTexture;
			}
			else {
				NewError("Too many Diffuse Textures!\n");
			}
		}

		unsigned int specularNum = material->GetTextureCount(aiTextureType_SPECULAR);
		printf("Specular Textures : %d\n", specularNum);
		for (int i = 0; i < specularNum; i++) {
			material->GetTexture(aiTextureType_SPECULAR, i, &textureName);
			printf("	|-> %s\n", textureName.C_Str());
		}
	}



	importer.FreeScene();
	
}


void Mesh::Draw()
{
	if (defaultShader != nullptr) defaultShader->UseShader();
	else {
		NewError("Default Shader is Null, create new shader with Shader(const char* vertexPath, const char* fragmentPath,true)");
		return;
	}
	VA->Bind();
	index->Bind();
	for (int i = 0; i < 32; i++) {
		if (texList[i] != nullptr) {
			texList[i]->Bind(i);

		}
		else {
			break;
		}
	}
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
