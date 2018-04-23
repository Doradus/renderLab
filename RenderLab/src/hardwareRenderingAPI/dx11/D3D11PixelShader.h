#pragma once

#pragma once
#include "PixelShader.h"
#include "D3D11Utils.h"

class D3D11PixelShader : public PixelShader {
public:
	D3D11PixelShader();
	~D3D11PixelShader();
	ID3D11PixelShader* resource;
};