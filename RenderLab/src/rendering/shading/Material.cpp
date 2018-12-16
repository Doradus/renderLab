#include "Material.h"

Material::Material() :
	albedo (nullptr),
	shader (nullptr)
{
}

Material::~Material() {
	std::vector<MaterialNode* >::iterator it;
	it = materialNodes.begin();

	while (it != materialNodes.end()) {
		MaterialNode* node = *it;
		materialNodes.erase(it);

		delete node;
		node = nullptr;
		it = materialNodes.begin();
	}

	std::vector<MaterialTextureUniform* >::iterator uniformIt;
	uniformIt = textureUniforms.begin();
	while (uniformIt != textureUniforms.end()) {
		MaterialTextureUniform* node = *uniformIt;
		textureUniforms.erase(uniformIt);

		delete node;
		node = nullptr;
		uniformIt = textureUniforms.begin();
	}

	if (shader != nullptr) {
		delete shader;
		shader = nullptr; 
	}
}

void Material::SetAlbedo(MaterialNode * value) {
	albedo = value;
}

void Material::SetSpecularColor(const XMFLOAT3 & value) {
	specularColor = value;
}

void Material::SetSpecularColor(float r, float g, float b) {
	specularColor.x = r;
	specularColor.y = g;
	specularColor.z = b;
}

void Material::SetSpecularPower(float value) {
	specularPower = value;
}

void Material::SetShader(PixelShader* inShader) {
	shader = inShader;
}

void Material::AddMaterialNode(MaterialNode * node) {
	node->SetOwner(this);
	materialNodes.push_back(node);
}

void Material::AddTextureUniform(MaterialTextureUniform * inUniform) {
	textureUniforms.push_back(inUniform);
}

PixelShader* Material::GetShader() const {
	return shader;
}

MaterialNode* Material::GetAlbedo() const {
	return albedo;
}

std::vector<MaterialTextureUniform*> Material::GetMaterialUniforms() const {
	return textureUniforms;
}

XMFLOAT3 Material::GetSpecularColor() const {
	return specularColor;
}

float Material::GetSpecularPower() const {
	return specularPower;
}
