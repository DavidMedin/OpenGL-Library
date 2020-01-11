#include "pch.h"
#include "SceneTree.h"

Attribute::Attribute(unsigned int type) {
	this->type = type;
}
Attribute::Attribute() {
	type = NULL;
}

SceneNode::SceneNode() {

}

void Attribute_AddMesh(SceneNode* node,aiScene* scene,unsigned int meshIndex) {
		SceneNode* currentNode = new SceneNode;
		node->childNodes.push_back(currentNode);

		aiMesh* aiTmpMesh = scene->mMeshes[i];
		Mesh* mesh = new Mesh(scene->mMeshes[i]);
		currentNode->attributeList.push_back((Attribute*)mesh);
		
		//texture loading
		unsigned int num_textures = scene->mNumTextures;
		unsigned int num_material = scene->mNumMaterials;
		aiMaterial* material = nullptr;
		Texture* diffuseTexture;
		aiString textureName;

		//gets the path to the textures
		size_t lastChar = node->path.find_last_of("/\\") + 1;
		string pathTo = node->path.erase(lastChar);
		 
		//checks if certain types of textures exist
		//then get that ONE texture and create an attribute about it
		if (scene->HasMaterials()) {
			material = scene->mMaterials[aiTmpMesh->mMaterialIndex];

			unsigned int diffuseNum = material->GetTextureCount(aiTextureType_DIFFUSE);
			//printf("Diffuse Textures : %d\n", diffuseNum);
			for (int i = 0; i < diffuseNum; i++) {
				if (diffuseNum == 1) {
					material->GetTexture(aiTextureType_DIFFUSE, i, &textureName);
					//printf("	|-> %s\n", textureName.C_Str());
					diffuseTexture = new Texture(DIFFUSE_SLOT, pathTo + string(textureName.C_Str()));
					currentNode->attributeList.push_back((Attribute*)diffuseTexture);
 				}
				else {
					NewError("Too many Diffuse Textures!\n");
				}
			}
			
			unsigned int specularNum = material->GetTextureCount(aiTextureType_SPECULAR);
			//printf("Specular Textures : %d\n", specularNum);
			for (int i = 0; i < specularNum; i++) {
				material->GetTexture(aiTextureType_SPECULAR, i, &textureName);
				//printf("	|-> %s\n", textureName.C_Str());
			}
		}

}

SceneNode::SceneNode(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (scene == NULL) {
		NewError(importer.GetErrorString());
		return;
	}
	this->path = path;
	aiNode* currentNode = scene->mRootNode;
	for (int i = 0; i < currentNode->mNumChildren; i++) {

	}
}


std::list<Attribute*>* SceneNode::GetAttributesOf(unsigned int type) {
	std::list<Attribute*>* typeList = new std::list<Attribute*>();
	for (Attribute* i : attributeList) {
		if (i->type == type) {
			typeList->push_back(i);
		}
	}
	return typeList;
}
void SceneNode::AddAttribute(Attribute* data, unsigned int dataType) {
	attributeList.push_back(data);
}
void SceneNode::RemoveAttribute(Attribute* data/*some way of identification*/) {
	attributeList.remove(data);
}

void SceneNode::Draw(Shader* shad, Camera* cam) {
	for (Attribute* i : attributeList) {
		if (i->type == ATTRIBUTE_TEXTURE) {
			Texture* tmpTex = (Texture*)i;
			tmpTex->Bind(0);
		}
	}
	for (Attribute* i : attributeList) {
		if (i->type == ATTRIBUTE_MESH) {
			Mesh* tmpMesh = (Mesh*)i;
			tmpMesh->Draw(shad, cam);
		}
	}
}