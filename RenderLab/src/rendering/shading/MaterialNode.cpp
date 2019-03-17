#include "MaterialNode.h"

MaterialNode::MaterialNode(Material* material) :
	owner (material),
	codeIndex (-1){}

MaterialNode::~MaterialNode() {}

TextureSamplerNode::TextureSamplerNode(Material* material, TextureTyes type) :
	textureType(type),
	MaterialNode(material) {}

TextureSamplerNode::~TextureSamplerNode() {}

void TextureSamplerNode::GetValue(DirectX::XMFLOAT4 * outValue) const {}

int TextureSamplerNode::Compile(MaterialCompiler * compiler) {
	if (codeIndex != -1) {
		return codeIndex;
	}

	int textureIndex = owner->GetTextureIndex(texture);
	codeIndex = compiler->TextureSampler(textureIndex, textureType);

	return codeIndex;
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


int Vector3Node::Compile(MaterialCompiler * compiler) {
	if (codeIndex != -1) {
		return codeIndex;
	}

	codeIndex = compiler->Vector3(R, G, B);

	return codeIndex;
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

int ScalarNode::Compile(MaterialCompiler * compiler) {
	if (codeIndex != -1) {
		return codeIndex;
	}

	codeIndex = compiler->Scalar(R);

	return codeIndex;
}

void ScalarNode::SetValues(float r) {
	R = r;
}

Multiply::Multiply(Material * material) :
	MaterialNode(material) {}

Multiply::~Multiply() {}

void Multiply::GetValue(DirectX::XMFLOAT4 * outValue) const {}

int Multiply::Compile(MaterialCompiler * compiler) {
	if (codeIndex != -1) {
		return codeIndex;
	}

	codeIndex = compiler->Multiply(a->Compile(compiler), b->Compile(compiler));

	return codeIndex;
}
