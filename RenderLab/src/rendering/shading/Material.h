#pragma once
#include "MathUtils.h"
#include "Shader.h"
#include "RenderingInterfaceResources.h"
#include "MaterialValue.h"
#include "MaterialUtils.h"
#include "MaterialNode.h"
#include "RenderingInterface.h"
using namespace DirectX;

class Material {
public:
	Material();
	~Material();

	void SetAlbedo(MaterialNode* node);
	void SetSpecularColor(const XMFLOAT3& value);
	void SetSpecularColor(float r, float g, float b);
	void SetSpecularPower(float value);
	void SetShader(PixelShader* inShader);

	TextureSamplerNode* CreateMaterialNode();

	MaterialNode*  GetAlbedo() const;
	XMFLOAT3 GetSpecularColor() const;
	float GetSpecularPower() const;
	PixelShader* GetShader() const;

private:
	MaterialNode* albedo;
	XMFLOAT3 specularColor;
	float specularPower;

	std::vector<MaterialNode*> materialNodes;

	ConstantBuffer* constantBuffer;
	PixelShader* shader;
	bool usingNormals;
	bool usingRougthness;
	bool usingMetalicness;
	bool usingTransparency;
	BlendModes blendMode;
};

