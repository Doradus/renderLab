#pragma once
#include "MathUtils.h"
using namespace DirectX;

class Material {
public:
	Material();
	~Material();

	void SetAlbedo(const XMFLOAT3& value);
	void SetAlbedo(float r, float g, float b);
	void SetSpecularColor(const XMFLOAT3& value);
	void SetSpecularColor(float r, float g, float b);
	void SetSpecularPower(float value);

	XMFLOAT3 GetAlbedo() const;
	XMFLOAT3 GetSpecularColor() const;
	float GetSpecularPower() const;

private:
	XMFLOAT3 albedo;
	XMFLOAT3 specularColor;
	float specularPower;
};