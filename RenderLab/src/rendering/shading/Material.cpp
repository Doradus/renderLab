#include "Material.h"

Material::Material()
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

PixelShader* Material::GetShader() const {
	return shader;
}

TextureSamplerNode * Material::CreateMaterialNode() {
	TextureSamplerNode* textureSamplerNode = new TextureSamplerNode();
	materialNodes.push_back(textureSamplerNode);

	return textureSamplerNode;
}

MaterialNode* Material::GetAlbedo() const {
	return albedo;
}

XMFLOAT3 Material::GetSpecularColor() const {
	return specularColor;
}

float Material::GetSpecularPower() const {
	return specularPower;
}
