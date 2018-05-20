#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::SetAlbedo(const XMFLOAT3 & value) {
	albedo = value;
}

void Material::SetAlbedo(float r, float g, float b) {
	albedo.x = r;
	albedo.y = g;
	albedo.z = b;
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

XMFLOAT3 Material::GetAlbedo() const {
	return albedo;
}

XMFLOAT3 Material::GetSpecularColor() const {
	return specularColor;
}

float Material::GetSpecularPower() const {
	return specularPower;
}
