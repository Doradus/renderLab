#pragma once
#include "RenderingInterfaceResources.h"

class TextureResource {
public:
	TextureResource() {};
	~TextureResource() {};

protected:
	TextureRI resource;
};

class TextureResource2D : public TextureResource {
public:
	TextureResource2D() {};
	~TextureResource2D() {};

private:

};