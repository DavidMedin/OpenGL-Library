#include "Mesh.h"
#define MAXBONEIDS 4

Mesh::Mesh(float* data, unsigned int size,unsigned int* indexData,unsigned int indexCount)
{
	drawMode = GL_TRIANGLES;
	vertices = data;
	pointCount = size / (sizeof(float) * 3);
	vertexBuffer = new VertexBuffer(data, size);
	VA = new VertexArray();
	VA->BindVertexBuffer(vertexBuffer, 3, GL_FLOAT, false);
	
	indices = indexData;
	this->indexCount = indexCount;
	index = new IndexBuffer(indexData, indexCount);
	//unused
	//will want to check if these buffers are defined if you want to use them in your shader
	textureUVBuffer = nullptr;
	normalBuffer = nullptr;
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


	//bones
	boneIds = (int*)malloc(pointCount * sizeof(int)*MAXBONEIDS);
	weights = (float*)calloc(pointCount * sizeof(float) * MAXBONEIDS, 1);//zeroed out
	for (unsigned int i = 0; i < pointCount*MAXBONEIDS; i++) {
		boneIds[i] = -1;
	}
	skelly = nullptr;
	if (mesh->HasBones()) {
		boneCount = mesh->mNumBones;
		if (boneCount > 32) {
			printf("WARNING - too many bones, things will be broken! (32 max, you have %d)\n", boneCount);
		}
		for (unsigned int i = 0; i < boneCount; i++) {
			const aiBone* bone = mesh->mBones[i];
			unsigned int weightNum = bone->mNumWeights;
			for (unsigned int u = 0; u < weightNum; u++) {
				aiVertexWeight weight = bone->mWeights[u];
				unsigned int vertexId = weight.mVertexId;
				if (weight.mWeight >= 0.3) {
					for (int q = 0; q < 3; q++) {
						if (boneIds[vertexId*MAXBONEIDS + q] != -1) continue;
						 boneIds[vertexId*MAXBONEIDS+q] = (int)i; // index through points to get the only bone it is affected by.
						weights[vertexId * MAXBONEIDS + q] = weight.mWeight;
						break;
					}
				}
			}
		}
		skelly = new Skeleton((aiNode*)scene->mRootNode, (aiMesh*)mesh,(aiScene*)scene);
	}
	else {
		printf("	No bones!\n");
	}
	boneIdBuffer = new VertexBuffer(boneIds, pointCount * sizeof(int)*MAXBONEIDS);
	VA->BindIntVertexBuffer(boneIdBuffer,4);
	weightBuffer = new VertexBuffer(weights, pointCount * sizeof(float) * MAXBONEIDS);
	VA->BindVertexBuffer(weightBuffer, 4, GL_FLOAT, GL_FALSE);

	//texture loading
	unsigned int num_textures = scene->mNumTextures;
	unsigned int num_material = scene->mNumMaterials;
	aiMaterial* material= nullptr;
	Texture* diffuseTexture;
	aiString textureName;

	size_t lastChar = path.find_last_of("/\\")+1;
	std::string pathTo = path.erase(lastChar);

	for (int i = 0; i < 32; i++) {
		textures[i] = nullptr;
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
			textures[DIFFUSE_SLOT] = diffuseTexture;
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
	for (int i = 0; i < 32; i++) {
		if (textures[i] != nullptr) {
			textures[i]->Bind(i);
		}
	}
	GLCall(glDrawElements(drawMode, indexCount, GL_UNSIGNED_INT, nullptr));
}