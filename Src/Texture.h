#pragma once

//glew
#include <GL/glew.h>

#include <string>

#include "GlCall.h"

#define DIFFUSE_SLOT 0
#define SPECULAR_SLOT 1

class Texture {
private:
	unsigned int openglID;
	unsigned int height, width;
	unsigned char* data;
	unsigned int slot;
public:
	Texture(unsigned int slot, unsigned char* data, unsigned int w, unsigned int h);
	Texture(unsigned int slot, std::string path);
	Texture();
	~Texture();
	void Bind(unsigned int slot = NULL);
};