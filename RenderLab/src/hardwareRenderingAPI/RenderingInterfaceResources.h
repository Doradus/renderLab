#pragma once
#ifndef RENDERING_INTERFACE_RESOURCES_H
#define RENDERING_INTERFACE_RESOURCES_H
#include <float.h>
#include <string>

enum TextureTyes {
	COLOR_MAP,
	NORMAL_MAP
};

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

enum Filtering {
	POINT_FILTERING,
	BILINEAR_FILTERING,
	TRILINEAR_FILTERING,
	ANISOTROPIC_FILTERING,
	COMPARE_BILINEAR_FILTERING
};

enum ShaderStages {
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER,
	NUM_SHADER_TYPES
};

struct ShaderMacro {
	ShaderMacro() {};
	std::string name;
	std::string definition;
};

struct SamplerConfig {
	SamplerConfig():
		filter (TRILINEAR_FILTERING),
		addressModeU(WRAP),
		addressModeV(WRAP),
		addressModeW(WRAP),
		mipLODBias (0.0f),
		maxAnisotropy (1),
		comparisonFunction (NEVER),
		minLOD(-FLT_MAX),
		maxLOD(FLT_MAX),
		borderColor ()
		{};
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

class TextureRI {
public:
	TextureRI() {};
	virtual ~TextureRI() {};

private:

};


class Texture2DRI : virtual public TextureRI {
public:
	Texture2DRI() {};
	virtual ~Texture2DRI() {};

private:

};

class RenderTargetInfo {
public :
	RenderTargetInfo(TextureRI* inTexture, unsigned int inArraySlice) :
		texture(inTexture),
		arraySlice (inArraySlice)
	{};
	~RenderTargetInfo() {};

	TextureRI* texture;
	unsigned int arraySlice;
};

#endif