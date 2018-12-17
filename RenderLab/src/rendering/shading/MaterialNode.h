#pragma once
#include <string>
#include "MathUtils.h"
#include "Material.h"
#include "RenderingInterfaceResources.h"

class Material;

class MaterialNode {
public:
	MaterialNode(Material* material);
	virtual ~MaterialNode();

	virtual void GetValue(DirectX::XMFLOAT4* outValue) const = 0;
	virtual void GenerateShaderCode(std::string* outValue) const  = 0;
	virtual std::string GetExpression() const = 0;

protected:
	Material* owner;

private:
	std::string name;
};

// ** textures ** //

class TextureSamplerNode : public MaterialNode {
public:
	TextureSamplerNode(Material* material);
	~TextureSamplerNode();

	virtual void GetValue(DirectX::XMFLOAT4* outValue) const override;
	virtual void GenerateShaderCode(std::string* outValue) const override;
	virtual std::string GetExpression() const override;

	TextureRI* GetTexture() const;

	void AddTexture(TextureRI* inTexture);

private:
	unsigned int textureIndex;
	TextureRI* texture;
};