#pragma once
#include <string>
#include "MathUtils.h"
#include "RenderingInterfaceResources.h"
class MaterialNode {
public:
	MaterialNode();
	virtual ~MaterialNode();

	virtual void GetValue(DirectX::XMFLOAT4* outValue) const = 0;
	virtual void GenerateShaderCode(std::string* outValue) const  = 0;

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

	void AddTexture(TextureRI* inTexture);

private:
	TextureRI* texture;
};