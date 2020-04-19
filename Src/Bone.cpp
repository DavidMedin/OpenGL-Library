#include "Bone.h"

#define GETMATRIX(i) skelly->boneMatrices[i->index]
#define GETOFFSET(i) skelly->boneOffsets[i->index]

void ConvertAssimpMatrix(glm::vec3* dest, aiMatrix4x4* m) {
	*dest = glm::vec3(m->a4, m->b4, m->c4);
	//return new mat4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	m.a4, m.b4, m.c4, m.d4
	//);
}

unsigned int boneInstances = 0;
BoneNode::BoneNode()
{
	boneInstances++;
	name = "BoneNode_" + std::to_string(boneInstances);
	
	index = NULL;

	parent = nullptr;
}

BoneNode::BoneNode(Skeleton* skelly, aiNode* node,aiMesh* mesh)
{
	//node should be the root scene node
	//looks for a match for any of the given names
	kill = false;
	parent = nullptr;
	index = -1;

	this->skelly = skelly;
	bool hasBone=false;
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		name = node->mName.C_Str();
		if (strcmp(mesh->mBones[i]->mName.C_Str(), node->mName.C_Str())==0) {//this node is one of our bones!
			hasBone = true;
			index = i;
			break;
		}
	}
	bool hasGoodBone = false;
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		BoneNode* child = new BoneNode(skelly,node->mChildren[i], mesh);
		if (child->kill != true) {//won't be null if has bones and stuff
			hasGoodBone = true;
			children.push_back(child);
			child->parent = this;
		}
		else delete child;
	}
	if (hasBone || hasGoodBone) {// this is a good node, populate it
		//ConvertAssimpMatrix(offset,&mesh->mBones[boneIndex]->mOffsetMatrix);
		//matrix = new glm::mat4(glm::identity<glm::mat4>());
		return;
	}
	kill = true;
}

void BoneNode::Rotate(glm::quat* quat) {
	if (index != -1) {
		//if there is no parent, assume indentity matrix
		glm::mat4 tmpParent = parent->index != -1 ? GETMATRIX(parent) : glm::identity<glm::mat4>();
		//apply the rotations
		//GETMATRIX(this) = tmpParent * glm::translate(glm::identity<glm::mat4>(),GETOFFSET(this)*glm::vec3(-1,-1,-1)) * glm::mat4_cast(*quat) * glm::translate(glm::identity<glm::mat4>(),GETOFFSET(this));
		GETMATRIX(this) = tmpParent*glm::translate(glm::identity<glm::mat4>(), GETOFFSET(this));

		for (BoneNode* child : children) {
			glm::quat* tmp = new glm::quat(glm::identity<glm::quat>());
			child->Rotate(tmp);
			delete tmp;
		}
	}
	else printf("This node isn't a bone! (index: %u)\n",index);
}

void BoneNode::Animate(double tick, glm::mat4* parMat)
{
	//find the before and after ticks for pos rot and sca
	//then find the interpolated result
	glm::vec3* translate = new glm::vec3(0);
	if (posKeyNum != 0) {
		int beforeKey=0;
		int afterKey=0;
		for (int k = 0; k < posKeyNum-1; k++) {
			beforeKey = k;
			afterKey = k + 1;
			if (posKeyTimes[afterKey] >= tick) {
				break;
			}
		}
		//weight=m*time-offset
		double m = -1 / (posKeyTimes[beforeKey] - posKeyTimes[afterKey]);
		double weight = m * tick-posKeyTimes[beforeKey];
		delete translate;
		//translate = new glm::vec3(glm::mix(posKeys[beforeKey], posKeys[afterKey],weight));
		translate = new glm::vec3(posKeys[beforeKey]);
	}

	glm::vec3* scale = new glm::vec3(0);
	if (scaKeyNum != 0) {
		int beforeKey = 0;
		int afterKey = 0;
		for (int k = 0; k < scaKeyNum - 1; k++) {
			beforeKey = k;
			afterKey = k + 1;
			if (scaKeyTimes[afterKey] >= tick) {
				break;
			}
		}
		//weight=m*time-offset
		double m = -1 / (scaKeyTimes[beforeKey] - scaKeyTimes[afterKey]);
		double weight = m * tick - scaKeyTimes[beforeKey];
		delete scale;
		scale = new glm::vec3(glm::mix(scaKeys[beforeKey], scaKeys[afterKey],weight));
	}

	glm::quat* rotation = new glm::quat(glm::identity<glm::quat>());
	if (rotKeyNum != 0) {
		int beforeKey = 0;
		int afterKey = 0;
		for (int k = 0; k < rotKeyNum - 1; k++) {
			beforeKey = k;
			afterKey = k + 1;
			if (rotKeyTimes[afterKey] >= tick) {
				break;
			}
		}
		//weight=m*time-offset
		double m = -1 / (rotKeyTimes[beforeKey] - rotKeyTimes[afterKey]);
		double weight = m * tick - rotKeyTimes[beforeKey];
		delete rotation;
		rotation = new glm::quat(glm::slerp(rotKeys[beforeKey], rotKeys[afterKey], (float)weight));
	}
	glm::mat4* childMat = new glm::mat4(glm::identity<glm::mat4>());
	if (index != -1) {
		glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), *translate);
		glm::mat4 sca = glm::scale(glm::identity<glm::mat4>(), *scale);
		glm::mat4 rot = glm::mat4_cast(*rotation);
		glm::mat4* transform = new glm::mat4(trans*rot*sca);
		glm::mat4 parentMat = parent->index != -1 ? GETMATRIX(parent) : glm::identity<glm::mat4>();

		delete childMat;
		childMat = new glm::mat4(glm::translate(*transform, GETOFFSET(this)));
		GETMATRIX(this) = parentMat */* glm::inverse(glm::translate(glm::identity<glm::mat4>(), GETOFFSET(this))) **/ *transform * glm::translate(glm::identity<glm::mat4>(), GETOFFSET(this));
		delete transform;
	}

	for (BoneNode* child : children) {
		child->Animate(tick,childMat);
	}
	delete rotation;
	delete scale;
	delete translate;
	delete childMat;
}


//============================================================


Skeleton::Skeleton()
{
	boneCount = 0;
	boneMatrices = nullptr;
	boneOffsets = nullptr;
	rootBone = nullptr;
}

Skeleton::Skeleton(aiNode* node, aiMesh* mesh, aiScene* scene)
{
	boneCount = mesh->mNumBones;
	if (boneCount > 32) {//will change when using uniform buffers
		printf("WARNING - too many bones, things will be broken! (32 max, you have %d)\n", boneCount);
	}
	//allocate memory
	boneMatrices = (glm::mat4*)malloc(sizeof(glm::mat4) * boneCount);
	boneOffsets = (glm::vec3*)malloc(sizeof(glm::vec3) * boneCount);
	//get where the bones are
	for (unsigned int i = 0; i < boneCount; i++) {
		const aiBone* bone = mesh->mBones[i];
		ConvertAssimpMatrix(&boneOffsets[i], (aiMatrix4x4*)&bone->mOffsetMatrix);
		boneMatrices[i] = glm::identity<glm::mat4>();
	}

	// create out bones
	rootBone = new BoneNode(this,node, mesh);
	//loading animation--------------------------
	if (scene->mNumAnimations > 0) {
		aiAnimation* anim = scene->mAnimations[0];
		printf("	animation name: %s\n", anim->mName.C_Str());
		printf("	animation has %u node channels\n", anim->mNumChannels);
		printf("	animation duration %f\n", anim->mDuration);
		printf("	ticks per second%f\n", anim->mTicksPerSecond);
		
		//will need to all of below as many times as there is animations ;(
		for (unsigned int i = 0; i < anim->mNumChannels; i++) {
			aiNodeAnim* chan = anim->mChannels[i];
			//get the corrosponding node from tree
			BoneNode* node = NameSearch(rootBone, chan->mNodeName.C_Str());
			
			node->posKeyNum = chan->mNumPositionKeys;
			node->rotKeyNum = chan->mNumRotationKeys;
			node->scaKeyNum = chan->mNumScalingKeys;

			node->posKeys = (glm::vec3*)malloc(sizeof(glm::vec3) * node->posKeyNum);
			node->rotKeys = (glm::quat*)malloc(sizeof(glm::quat) * node->rotKeyNum);
			node->scaKeys = (glm::vec3*)malloc(sizeof(glm::vec3) * node->scaKeyNum);
		
			node->posKeyTimes = (double*)malloc(sizeof(double) * node->posKeyNum);
			node->rotKeyTimes = (double*)malloc(sizeof(double) * node->rotKeyNum);
			node->scaKeyTimes = (double*)malloc(sizeof(double) * node->scaKeyNum);

			for (unsigned int p = 0; p < node->posKeyNum; p++) {
				aiVectorKey key = chan->mPositionKeys[p];
				aiVector3D vec = key.mValue;
				node->posKeys[p].x = vec.x;
				node->posKeys[p].y = vec.y;
				node->posKeys[p].z = vec.z;
				node->posKeyTimes[p] = key.mTime;
			}
			for (unsigned int r = 0; r < node->rotKeyNum; r++) {
				aiQuatKey key = chan->mRotationKeys[r];
				aiQuaternion quat = key.mValue;
				node->rotKeys[r].x = quat.x;
				node->rotKeys[r].y = quat.y;
				node->rotKeys[r].z = quat.z;
				node->rotKeys[r].w = quat.w;
				node->rotKeyTimes[r] = key.mTime;
			}
			for (unsigned int s = 0; s < node->scaKeyNum; s++) {
				aiVectorKey key = chan->mScalingKeys[s];
				aiVector3D vec = key.mValue;
				node->scaKeys[s].x = vec.x;
				node->scaKeys[s].y = vec.y;
				node->scaKeys[s].z = vec.z;
				node->scaKeyTimes[s] = key.mTime;
			}
		}
	}

}

void Skeleton::Rotate(unsigned int index, glm::quat* quat)
{
	BoneNode* child = IndexSearch(rootBone, index);
	if (child != nullptr) {
		child->Rotate(quat);
	}
	else printf("There is no bone of index %u in this skeleton!\n", index);
}


void Skeleton::Animate(double tick)
{
	glm::mat4* parMat = new glm::mat4(glm::identity<glm::mat4>());
	rootBone->Animate(tick,parMat);
	delete parMat;
}


BoneNode* Skeleton::IndexSearch(BoneNode* node,int index) {
	if (node->index == index) { //this is the correct node
		return node;
	}
	else {
		for (BoneNode* child : node->children) { // check children for correct node
			BoneNode* rez = IndexSearch(child, index);//  check if this child is good
			if (rez != nullptr) {
				return rez;
			}
		}
		return nullptr; // if fail return nullptr
	}
}

BoneNode* Skeleton::NameSearch(BoneNode* node, std::string name)
{
	if (node->name == name) { //this is the correct node
		return node;
	}
	else {
		for (BoneNode* child : node->children) { // check children for correct node
			BoneNode* rez = NameSearch(child, name);//  check if this child is good
			if (rez != nullptr) {
				return rez;
			}
		}
		return nullptr; // if fail return nullptr
	}
}
