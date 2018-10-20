#pragma once
#include "Image.h"

struct TextureFormatInfo {
	ImageFormats::Format imageFormat;
	bool isSupported;
	unsigned int blockXSize;
	unsigned int blockYSize;
	unsigned int platformFormat;
};

enum TextureBindFlags {
	None = 0,
	TextureBindAsShaderResource = 1 << 0,
	TextureBindAsRenderTarget = 1 << 1,
	TextureBindAsDepthStencil = 1 << 2,
	CreateRTVArraySlicesIndividualy = 1 << 3
};

extern TextureFormatInfo GTextureFormatInfo[32];