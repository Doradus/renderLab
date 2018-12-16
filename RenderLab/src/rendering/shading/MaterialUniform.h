#pragma once
#include "MathUtils.h"
#include "SamplerState.h"
#include "RenderingInterfaceResources.h"
#include "RenderingInterface.h"
#include <vector>
class MaterialUniform {
public:
	MaterialUniform() {};
	virtual ~MaterialUniform(){};

	virtual void BindResource() const = 0;
};

class MaterialTextureUniform : public MaterialUniform {
public:
	MaterialTextureUniform(TextureRI* inTexture, unsigned int textureIndex);
	~MaterialTextureUniform();                             

	void AddTexture(TextureRI* inTexture, unsigned int textureIndex);

	virtual void BindResource() const override;
private:
	TextureRI* texture;
	unsigned int inputSlot;
};

class MaterialSamplerUniform : public MaterialUniform {
public:
	MaterialSamplerUniform();
	~MaterialSamplerUniform();

	virtual void BindResource() const override;

private:
	unsigned int inputSlot;
	SamplerState* sampler;
};
