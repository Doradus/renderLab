#pragma once
#include "MathUtils.h"
#include "Shader.h"
#include "RenderingInterfaceResources.h"
#include "MaterialValue.h"
#include "MaterialUtils.h"
#include "RenderingInterface.h"
using namespace DirectX;

class MaterialNode;
class MaterialUniform;
class MaterialTextureUniform;
class Material {
public:
	Material();
	~Material();

	void									SetAlbedo(MaterialNode* node);
	void									SetNormal(MaterialNode* node);
	void									SetSpecularColor(const XMFLOAT3& value);
	void									SetSpecularColor(float r, float g, float b);
	void									SetSpecularPower(float value);
	void									SetShader(PixelShader* inShader);
	void									AddMaterialNode(MaterialNode* node);
	void									AddTextureUniform(MaterialTextureUniform* inUniform);
	void									AddTexture(TextureRI* texture);

	MaterialNode*							GetAlbedo() const;
	MaterialNode*							GetNormal() const;
	std::vector<MaterialTextureUniform*>	GetMaterialUniforms() const;
	std::vector<TextureRI*>					GetTextureResources() const;
	XMFLOAT3								GetSpecularColor() const;
	float									GetSpecularPower() const;
	PixelShader*							GetShader() const;
	int										GetTextureIndex(TextureRI* texture) const;
	unsigned int							GetNumberOfTexturesUsed() const;
	bool									UseNormals() const;

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
	MaterialNode*							normal;
	XMFLOAT3								specularColor;
	float									specularPower;

	std::vector<MaterialNode*>				materialNodes;
	std::vector<MaterialTextureUniform*>	textureUniforms;
	std::vector<TextureRI*>					textures;
	ConstantBuffer*							constantBuffer;
	PixelShader*							shader;

	bool									usingRougthness;
	bool									usingMetalicness;
	bool									usingTransparency;
	BlendModes								blendMode;
};

