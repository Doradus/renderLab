#include "MaterialNode.h"

MaterialNode::MaterialNode(Material* material) :
 owner (material) {}

MaterialNode::~MaterialNode() {}

TextureSamplerNode::TextureSamplerNode(Material* material) :
 MaterialNode(material) {}

TextureSamplerNode::~TextureSamplerNode() {}

void TextureSamplerNode::GetValue(DirectX::XMFLOAT4 * outValue) const {}

void TextureSamplerNode::GenerateShaderCode(std::string * outValue) const {}

std::string TextureSamplerNode::GetExpression() const {
	return "return texture00.Sample(textureSampler, vIn.uv).rgb;";
}

TextureRI * TextureSamplerNode::GetTexture() const {
	return texture;
}

void TextureSamplerNode::AddTexture(TextureRI * inTexture) {
	texture = inTexture;
	owner->AddTexture(inTexture);
}
