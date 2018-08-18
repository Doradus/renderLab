#pragma once
#include "GeometryShader.h"
#include "D3D11Utils.h"

class D3D11GeometryShader : public GeometryShader {
public:
	D3D11GeometryShader(ID3D11GeometryShader* inResource);
	~D3D11GeometryShader();

public:
	ID3D11GeometryShader* resource;
};
