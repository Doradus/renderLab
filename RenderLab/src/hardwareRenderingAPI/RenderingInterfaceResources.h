#pragma once
#ifndef RENDERING_INTERFACE_RESOURCES_H
#define RENDERING_INTERFACE_RESOURCES_H

enum AddressModes {
	WRAP,
	CLAMP,
	BORDER,
	MIRROR
};

enum SamplerCompareFunction {
	NEVER,
	LESS,
	LESS_OR_EQUAL,
	EQUAL,
	NOT_EQUAL,
	GREAT_OR_EQUAL,
	GREATER,
	ALWAYS
};

struct SamplerConfig {
	SamplerConfig() {};
	unsigned int filter;
	AddressModes addressModeU;
	AddressModes addressModeV;
	AddressModes addressModeW;
	float mipLODBias;
	unsigned int maxAnisotropy;
	SamplerCompareFunction comparisonFunction;
	float borderColor[4];
	float minLOD;
	float maxLOD;
};

enum Filtering {
	POINT_FILTERING,
	BILINEAR_FILTERING,
	TRILINEAR_FILTERING,
	ANISOTROPIC_FILTERING
};

class TextureRI {
public:
	TextureRI() {};
	virtual ~TextureRI() {};

private:

};


class Texture2DRI : public TextureRI {
public:
	Texture2DRI() {};
	virtual ~Texture2DRI() {};

private:

};

#endif