 
#include "Mesh.h"
#include "Init.h"

//stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int drawFlags = 1;


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
	//type = ATTRIBUTE_TEXTURE;
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
	//type = ATTRIBUTE_TEXTURE;

}

Texture::Texture() {
	slot = NULL;
	openglID = NULL;
	data = nullptr;
	height = NULL;
	width = NULL;
	//type = ATTRIBUTE_TEXTURE;

}
void Texture::Bind(unsigned int slot) {
	//will need a system for slot management for any given object
	unsigned int tmpSlot;
	if (slot == NULL) { tmpSlot = this->slot; }
	else { tmpSlot = slot; printf("improper use of Bind()\n"); }
	GLCall(glBindTexture(GL_TEXTURE_2D, openglID));
	GLCall(glActiveTexture(GL_TEXTURE0 + tmpSlot));
}
Texture::~Texture() {
	stbi_image_free(this->data);
}


//void ConvertAssimpMatrix(vec3* dest,aiMatrix4x4* m) {
//	*dest = vec3(m->a4, m->b4, m->c4);
//	//return new mat4(
//	//	1.0f, 0.0f, 0.0f, 0.0f,
//	//	0.0f, 1.0f, 0.0f, 0.0f,
//	//	0.0f, 0.0f, 1.0f, 0.0f,
//	//	m.a4, m.b4, m.c4, m.d4
//	//);
//}


Mesh::Mesh(void* mesh) {
	drawMode = GL_TRIANGLES;
	for (int i = 0; i < 32; i++) {
		texList[i] = nullptr;
	}

	aiMesh* tmpMesh = (aiMesh*)mesh;

	//vertices
	unsigned int point_count = tmpMesh->mNumVertices;
	float* points = nullptr;
	if (tmpMesh->HasPositions()) {
		points = (float*)malloc(sizeof(float) * point_count * 3);
		if (points != nullptr && point_count != 0) {
			for (unsigned int i = 0; i < point_count; i++) {
				aiVector3D vector = tmpMesh->mVertices[i];
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
	transform = new glm::mat4(1.0f);
	vertexBuffer = new VertexBuffer(points, sizeof(float) * point_count * 3);
	pointCount = point_count;
	VA = new VertexArray();
	VA->BindVertexBuffer(vertexBuffer, 3, GL_FLOAT, false);
	vertices = points;


	//index buffer
	unsigned int* indices = nullptr;
	unsigned int face_count = tmpMesh->mNumFaces;
	if (tmpMesh->HasFaces() && face_count >= 0) {
		indices = (unsigned int*)malloc(sizeof(unsigned int) * face_count * 3);
		if (indices != NULL) {
			for (int i = 0; i < (int)face_count; i++) {
				aiFace face = tmpMesh->mFaces[i];
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
	index = new IndexBuffer(indices, face_count * 3);
	this->indexCount = face_count * 3;
	this->indices = indices;

	//normals
	//use point_count for size
	float* normals = nullptr;
	if (tmpMesh->HasNormals() && point_count >= 0) {
		normals = (float*)malloc(sizeof(float) * 3 * point_count);
		if (normals != NULL) {
			for (unsigned int i = 0; i < point_count; i++) {
				aiVector3D vector = tmpMesh->mNormals[i];
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
	normalBuffer = new VertexBuffer(normals, point_count * 3 * sizeof(float));
	VA->BindVertexBuffer(normalBuffer, 3, GL_FLOAT, false);
	this->normals = normals;

	//texture UVs
	float* UVs = nullptr;
	if (tmpMesh->HasTextureCoords(0) && point_count >= 0) {
		UVs = (float*)malloc(sizeof(float) * 2 * point_count);
		if (UVs != NULL) {
			for (unsigned int i = 0; i < point_count; i++) {
				UVs[i * 2 + 0] = tmpMesh->mTextureCoords[0][i].x;
				UVs[i * 2 + 1] = tmpMesh->mTextureCoords[0][i].y;
			}
		}
		else {
			NewError("UVs' memory was incorrectly allocated\n");
		}
	}
	textureUVBuffer = new VertexBuffer(UVs, sizeof(float) * point_count * 2);
	VA->BindVertexBuffer(textureUVBuffer, 2, GL_FLOAT, false);
	this->textureUVs = UVs;

	//type = ATTRIBUTE_MESH;
}





Mesh::Mesh(float* data, unsigned int size)
{
	drawMode = GL_TRIANGLES;
	vertices = data;
	pointCount = size/sizeof(float)*3;
	vertexBuffer = new VertexBuffer(data, size);
	VA = new VertexArray();
	VA->BindVertexBuffer(vertexBuffer, 3, GL_FLOAT, false);
	
	indices = (unsigned int*)malloc(sizeof(unsigned int));
	*indices = 1;
	index = new IndexBuffer(indices, 1);
	indexCount = 1;

	//unused
	//will want to check if these buffers are defined if you want to use them in your shader
	textureUVBuffer = nullptr;
	normalBuffer = nullptr;
	transform = nullptr;
	boneCount = NULL;
	normals = nullptr;
	textureUVs = nullptr;
}

Mesh::Mesh(std::string path)
{	
	drawMode = GL_TRIANGLES;
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
	transform = new glm::mat4(1.0f);
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
			for (unsigned int i = 0; i < point_count; i++) {
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

	boneIds = (int*)malloc(pointCount * sizeof(int));
	for (unsigned int i = 0; i < pointCount; i++) {
		boneIds[i] = -1;
	}
	skelly = nullptr;
	if (mesh->HasBones()) {
		boneCount = mesh->mNumBones;
		if (boneCount > 32) {
			printf("WARNING - too many bones, things will be broken! (32 max, you have %d\n", boneCount);
		}
		for (unsigned int i = 0; i < boneCount; i++) {
			const aiBone* bone = mesh->mBones[i];
			unsigned int weightNum = bone->mNumWeights;
			for (unsigned int u = 0; u < weightNum; u++) {
				aiVertexWeight weight = bone->mWeights[u];
				unsigned int vertexId = weight.mVertexId;
				if (weight.mWeight >= 0.3) {
					boneIds[vertexId] = i; // index through points to get the only bone it is affected by.
					//because of this boneIds will be spotty
				}
			}
		}
		skelly = new Skeleton((aiNode*)scene->mRootNode, (aiMesh*)mesh,(aiScene*)scene);
	}
	else {
		printf("	No bones!\n");
	}
	boneIdBuffer = new VertexBuffer(boneIds, pointCount * sizeof(int));
	VA->BindIntVertexBuffer(boneIdBuffer);

	//texture loading
	unsigned int num_textures = scene->mNumTextures;
	unsigned int num_material = scene->mNumMaterials;
	aiMaterial* material= nullptr;
	Texture* diffuseTexture;
	aiString textureName;

	size_t lastChar = path.find_last_of("/\\")+1;
	std::string pathTo = path.erase(lastChar);

	for (int i = 0; i < 32; i++) {
		texList[i] = nullptr;
	}


	if (scene->HasMaterials()) {
		material = scene->mMaterials[mesh->mMaterialIndex];

		unsigned int diffuseNum = material->GetTextureCount(aiTextureType_DIFFUSE);
		//printf("Diffuse Textures : %d\n", diffuseNum);
		for (unsigned int i = 0; i < diffuseNum; i++) {
			if (diffuseNum == 1) {
			material->GetTexture(aiTextureType_DIFFUSE, i, &textureName);
			printf("	texture[%d]: %s\n",i, textureName.C_Str());
			diffuseTexture = new Texture(DIFFUSE_SLOT,pathTo + std::string(textureName.C_Str()));
			texList[DIFFUSE_SLOT] = diffuseTexture;
			}
			else {
				NewError("Too many Diffuse Textures!\n");
			}
		}

		unsigned int specularNum = material->GetTextureCount(aiTextureType_SPECULAR);
		//printf("Specular Textures : %d\n", specularNum);
		for (unsigned int i = 0; i < specularNum; i++) {
			material->GetTexture(aiTextureType_SPECULAR, i, &textureName);
			//printf("	|-> %s\n", textureName.C_Str());
		}
	}



	importer.FreeScene();
	//type = ATTRIBUTE_MESH;
}



void Mesh::Draw(Shader* shad,Camera* cam)
{
	shad->UseShader();
	shad->UniformEquals("proj",GL_FLOAT_MAT4, cam->projectionMatrix,1);
	shad->UniformEquals("view", GL_FLOAT_MAT4, cam->viewMat,1); 
	if (boneCount > 0) {
		shad->UniformEquals("bones", GL_FLOAT_MAT4, skelly->boneMatrices,skelly->boneCount);
	}
	VA->Bind();
	index->Bind();
	////if (GetGraphicsFlag(GRAPHICS_FLAG_CULL)) {
	////	glEnable(GL_CULL_FACE);
	////}
	//else glDisable(GL_CULL_FACE);
	for (int i = 0; i < 32; i++) { //was commented?
		if (texList[i] != nullptr) {
			texList[i]->Bind(i);
		}
		else {
			break;
		}
	}
		GLCall(glDrawElements(drawMode, indexCount, GL_UNSIGNED_INT, nullptr));
}
void Mesh::BindCustomData(VertexBuffer* data, unsigned int type,unsigned int vecX) {
	VA->BindCustomBuffer(data, vecX, type, false);
}

void Mesh::Bind() {
	VA->Bind();
	index->Bind();
}

Transform::Transform()
{
	//type = ATTRIBUTE_TRANSFORM;
	data = new glm::mat4(glm::identity<glm::mat4>());
}

Transform::Transform(glm::mat4* transform)
{
	data = transform;
	//type = ATTRIBUTE_TRANSFORM;
}

 void DrawFlags(int flag)
{
	drawFlags = drawFlags^ flag;
}

 bool GetDrawFlags(int flag)
{
	return drawFlags&flag;
}

 Line::Line()
 {
	points = (float*)malloc(sizeof(float) * 6);
	if (points != NULL) {
		points[0] = 0.0f;
		points[1] = 0.0f;
		points[2] = 0.0f;
		points[3] = 1.0f;
		points[4] = 1.0f;
		points[5] = 1.0f;
		VB = new VertexBuffer(points,sizeof(float)*6);
		VA = new VertexArray();
		//mappedPoints = (float*)VB->MapData();
		VA->BindVertexBuffer(VB, 3, GL_FLOAT, false);
		color = glm::vec3(1, 1, 1);
		size = 5;
	}
	else {
		printf("Could not allocate mem\n");
		return;
	}
 }
 Line::Line(glm::vec3 point1, glm::vec3 point2)
 {
	 points = (float*)malloc(sizeof(float) * 6);
	 if (points != NULL) {
		 points[0] = point1.x;
		 points[1] = point1.y;
		 points[2] = point1.z;
		 points[3] = point2.x;
		 points[4] = point2.y;
		 points[5] = point2.z;
		 VB = new VertexBuffer(points, sizeof(float) * 6);
		 VA = new VertexArray();
		 //mappedPoints = (float*)VB->MapData();
		 VA->BindVertexBuffer(VB, 3, GL_FLOAT, false);
		 color = glm::vec3(1, 1, 1);
		 size = 5;
	 }
	 else {
		 printf("Could not allocate mem\n");
		 return;
	 }
 }
 Line::Line(glm::vec3 point1, glm::vec3 point2,glm::vec3 color)
 {
	points = (float*)malloc(sizeof(float) * 6);
	if (points != NULL) {
		points[0] = point1.x;
		points[1] = point1.y;
		points[2] = point1.z;
		points[3] = point2.x;
		points[4] = point2.y;
		points[5] = point2.z;
		VB = new VertexBuffer(points, sizeof(float) * 6);
		VA = new VertexArray();
		//mappedPoints = (float*)VB->MapData();
		VA->BindVertexBuffer(VB, 3, GL_FLOAT, false);
		this->color = color;
		size = 5;
	}
	else {
		printf("Could not allocate mem\n");
		return;
	}
 }

 void Line::SetPoint1(glm::vec3 point)
 {
	 points[0] = point.x;
	 points[1] = point.y;
	 points[2] = point.z;
	 float* mem = (float*)VB->MapData();
	 memcpy(mem, points, sizeof(float) * 3);
	 VB->UnmapData();
 }

 void Line::SetPoint2(glm::vec3 point)
 {
	 points[3] = point.x;
	 points[4] = point.y;
	 points[5] = point.z;
	 float* mem = (float*)VB->MapData();
	 memcpy(&mem[3], &points[3], sizeof(float) * 3);
	 VB->UnmapData();
 }

 glm::vec3 Line::GetPoint1()
 {
	 return glm::vec3(points[0],points[1],points[2]);
 }

 glm::vec3 Line::GetPoint2()
 {
	 return glm::vec3(points[3],points[4],points[5]);
 }

 void Line::Draw(Camera* cam)
 {
	 glLineWidth(size);
	 Shader** shads = GetShaders();
	 shads[0]->UseShader();
	 shads[0]->UniformEquals("proj", GL_FLOAT_MAT4, cam->projectionMatrix,1);
	 shads[0]->UniformEquals("view", GL_FLOAT_MAT4, cam->viewMat,1);
	 VA->Bind();
	 GLCall(glDrawArrays(GL_LINES, 0, 1));
 }

 void Line::Draw(Shader* shad, Camera* cam)
 {
	 glLineWidth(size);
	 VA->Bind();
	 shad->UseShader();
	 shad->UniformEquals("proj", GL_FLOAT_MAT4, cam->projectionMatrix,1);
	 shad->UniformEquals("view", GL_FLOAT_MAT4, cam->viewMat,1);
	 shad->UniformEquals("color", GL_FLOAT_VEC3, &color[0],1);
	 GLCall(glDrawArrays(GL_LINES, 0, 2));
 }

 float* Line::OpenWriting()
 {
	mappedPoints = (float*)VB->MapData();
	return mappedPoints;
 }

 unsigned int Line::ClosedWriting()
 {
	 return VB->UnmapData();
 }
