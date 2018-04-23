#pragma once
#include "VertexShader.h"
#include "D3D11Utils.h"

class D3D11VertexShader : public VertexShader {
public:
	D3D11VertexShader();
	~D3D11VertexShader();
	ID3D11VertexShader* resource;
};