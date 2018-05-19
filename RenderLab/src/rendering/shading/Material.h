#pragma once
#include "MathUtils.h"
using namespace DirectX;

class Material {
public:
	Material();
	~Material();

	void SetAlbedo(const XMFLOAT3& value);
	void SetAlbedo(float r, float g, float b);

	XMFLOAT3 GetAlbedo() const;

private:
	XMFLOAT3 albedo;
};