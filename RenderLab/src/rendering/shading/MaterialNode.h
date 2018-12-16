#pragma once
#include <string>
#include "MathUtils.h"
#include "RenderingInterfaceResources.h"

class Material;

class MaterialNode {
public:
	MaterialNode();
	virtual ~MaterialNode();

	virtual void GetValue(DirectX::XMFLOAT4* outValue) const = 0;
	virtual void GenerateShaderCode(std::string* outValue) const  = 0;
	virtual std::string GetExpression() const = 0;

	void		SetOwner(Material* newOwner);

protected:
	Material* owner;

private:
	std::string name;
};

// ** textures ** //

class TextureSamplerNode : public MaterialNode {
public:
	TextureSamplerNode();
	~TextureSamplerNode();

	virtual void GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual void GenerateShaderCode(std::string* outValue) const override;
	virtual std::string GetExpression() const override;

	TextureRI* GetTexture() const;

	void AddTexture(TextureRI* inTexture);
	void SetTextureIndex(unsigned int usedIndex);

private:
	unsigned int textureIndex;
	TextureRI* texture;
};