#pragma once
#include "MathUtils.h"
#include "SamplerState.h"
#include "RenderingInterfaceResources.h"
#include "RenderingInterface.h"
#include "Material.h"
#include <vector>

class MaterialUniform {
public:
	MaterialUniform(Material* inMaterial);
	virtual ~MaterialUniform();

	virtual void BindResource() const = 0;

protected:
	Material* owner;
};

class MaterialTextureUniform : public MaterialUniform {
public:
	MaterialTextureUniform(TextureRI* inTexture, unsigned int textureIndex, Material* inMaterial);
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
