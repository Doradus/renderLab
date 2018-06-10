#pragma once
#include "TextureResource.h"

class Texture {
public:
	Texture();
	~Texture();

protected:
	TextureResource* textureResource;
};