#include "Material.h"

Material::Material() :
	albedo (nullptr),
	normal (nullptr),
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

void Material::SetNormal(MaterialNode * node) {
	normal = node;
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
	materialNodes.push_back(node);
}

void Material::AddTextureUniform(MaterialTextureUniform * inUniform) {
	textureUniforms.push_back(inUniform);
}

void Material::AddTexture(TextureRI* texture) {
	bool isSame = false;

	for (std::vector<TextureRI*>::size_type i = 0; i != textures.size(); i++) {
		if (texture == textures[i]) {
			isSame = true;
			break;
		}
	}

	if (!isSame) {
		textures.push_back(texture);
	}
}

PixelShader* Material::GetShader() const {
	return shader;
}

int Material::GetTextureIndex(TextureRI* texture) const {
	for (std::vector<TextureRI*>::size_type i = 0; i != textures.size(); i++) {
		if (texture == textures[i]) {
			return i;
		}
	}

	return -1;
}

bool Material::UseNormals() const {
	return normal != nullptr;
}

MaterialNode* Material::GetAlbedo() const {
	return albedo;
}

MaterialNode * Material::GetNormal() const {
	return normal;
}

std::vector<MaterialTextureUniform*> Material::GetMaterialUniforms() const {
	return textureUniforms;
}

std::vector<TextureRI*> Material::GetTextureResources() const {
	return textures;
}

XMFLOAT3 Material::GetSpecularColor() const {
	return specularColor;
}

float Material::GetSpecularPower() const {
	return specularPower;
}
