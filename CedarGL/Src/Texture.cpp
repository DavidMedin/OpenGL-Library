#include "Texture.h"
//stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(unsigned int slot, unsigned char* data, unsigned int w, unsigned int h) {
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
