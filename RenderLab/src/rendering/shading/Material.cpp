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

XMFLOAT3 Material::GetAlbedo() const {
	return albedo;
}
