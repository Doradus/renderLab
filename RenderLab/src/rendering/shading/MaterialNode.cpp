#include "MaterialNode.h"

MaterialNode::MaterialNode(Material* material) :
 owner (material) {}

MaterialNode::~MaterialNode() {}

TextureSamplerNode::TextureSamplerNode(Material* material) :
 MaterialNode(material) {}

TextureSamplerNode::~TextureSamplerNode() {}

void TextureSamplerNode::GetValue(DirectX::XMFLOAT4 * outValue) const {}

std::string TextureSamplerNode::GetExpression() const {
	int textureIndex = owner->GetTextureIndex(texture);

	return "return texture0" + std::to_string(textureIndex) + ".Sample(textureSampler, vIn.uv).rgb;";
}

TextureRI * TextureSamplerNode::GetTexture() const {
	return texture;
}

void TextureSamplerNode::AddTexture(TextureRI * inTexture) {
	texture = inTexture;
	owner->AddTexture(inTexture);
}

Vector3Node::Vector3Node(Material * material) :
MaterialNode (material) {}

Vector3Node::~Vector3Node() {}

void Vector3Node::GetValue(DirectX::XMFLOAT4 * outValue) const {}

std::string Vector3Node::GetExpression() const {
	return "return float3(" + std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + ");";
}

void Vector3Node::SetValues(float r, float g, float b) {
	R = r;
	G = g;
	B = b;
}
