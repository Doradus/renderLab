#pragma once
#include "MathUtils.h"
#include "Shader.h"
#include "RenderingInterfaceResources.h"
#include "MaterialValue.h"
#include "MaterialUtils.h"
#include "MaterialUniform.h"
#include "RenderingInterface.h"
using namespace DirectX;

class MaterialNode;
class Material {
public:
	Material();
	~Material();

	void									SetAlbedo(MaterialNode* node);
	void									SetSpecularColor(const XMFLOAT3& value);
	void									SetSpecularColor(float r, float g, float b);
	void									SetSpecularPower(float value);
	void									SetShader(PixelShader* inShader);
	void									AddMaterialNode(MaterialNode* node);
	void									AddTextureUniform(MaterialTextureUniform* inUniform);
	void									AddTexture(TextureRI* texture);

	MaterialNode*							GetAlbedo() const;
	std::vector<MaterialTextureUniform*>	GetMaterialUniforms() const;
	std::vector<TextureRI*>					GetTextureResources() const;
	XMFLOAT3								GetSpecularColor() const;
	float									GetSpecularPower() const;
	PixelShader*							GetShader() const;

	template <typename NodeType>
	void GetAllNodesOfType(std::vector<const NodeType*>& outNodes) const {
		for (unsigned int i = 0; i < materialNodes.size(); i++) {
			NodeType* nodeTypePtr = dynamic_cast<NodeType*>(materialNodes[i]);

			if (nodeTypePtr) {
				outNodes.push_back(nodeTypePtr);
			}
		}
	}

private:
	MaterialNode*							albedo;
	XMFLOAT3								specularColor;
	float									specularPower;

	std::vector<MaterialNode*>				materialNodes;
	std::vector<MaterialTextureUniform*>	textureUniforms;
	std::vector<TextureRI*>					textures;
	ConstantBuffer*							constantBuffer;
	PixelShader*							shader;

	bool									usingNormals;
	bool									usingRougthness;
	bool									usingMetalicness;
	bool									usingTransparency;
	BlendModes								blendMode;
};

