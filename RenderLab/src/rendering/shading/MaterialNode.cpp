#include "MaterialNode.h"

MaterialNode::MaterialNode() : 
 owner (nullptr) {}

MaterialNode::~MaterialNode() {}

void MaterialNode::SetOwner(Material * newOwner) {
	owner = newOwner;
}

TextureSamplerNode::TextureSamplerNode() {}

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
}

void TextureSamplerNode::SetTextureIndex(unsigned int usedIndex) {
	textureIndex = usedIndex;
}
