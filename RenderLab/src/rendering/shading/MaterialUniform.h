#pragma once
#include "MathUtils.h"
#include "SamplerState.h"
#include "RenderingInterfaceResources.h"
class MaterialUniform {
public:
	MaterialUniform() {};
	virtual ~MaterialUniform(){};

	virtual void BindResource() const = 0;
	virtual void GenerateShaderCode(std::string* outValue) const = 0;
};

class MaterialTextureUniform : public MaterialUniform {
public:
	MaterialTextureUniform();
	~MaterialTextureUniform();

private:
	unsigned int inputSlot;
	TextureRI* texture;
};

class MaterialSamplerUniform : public MaterialUniform {
public:
	MaterialSamplerUniform();
	~MaterialSamplerUniform();

private:
	unsigned int inputSlot;
	SamplerState* sampler;
};
