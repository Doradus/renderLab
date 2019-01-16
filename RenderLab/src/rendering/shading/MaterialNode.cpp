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

	return "return texture0" + std::to_string(textureIndex) + ".Sample(textureSampler, vIn.uv);";
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
	return "return float4(" + std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + ", 1.0f);";
}

void Vector3Node::SetValues(float r, float g, float b) {
	R = r;
	G = g;
	B = b;
}

ScalarNode::ScalarNode(Material * material) :
	MaterialNode(material) {}

ScalarNode::~ScalarNode() {}

void ScalarNode::GetValue(DirectX::XMFLOAT4 * outValue) const {}

std::string ScalarNode::GetExpression() const {
	return "return float4(" + std::to_string(R) + ", " + std::to_string(R) + ", " + std::to_string(R) + ", " + std::to_string(R) + ");";
}