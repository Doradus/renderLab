#include "MaterialNode.h"

MaterialNode::MaterialNode() {}

MaterialNode::~MaterialNode() {}

TextureSamplerNode::TextureSamplerNode() {}

TextureSamplerNode::~TextureSamplerNode() {}

void TextureSamplerNode::GetValue(DirectX::XMFLOAT4 * outValue) const {}

void TextureSamplerNode::GenerateShaderCode(std::string * outValue) const {}

void TextureSamplerNode::AddTexture(TextureRI * inTexture) {
	texture = inTexture;
}


