#pragma once
#include "Shader.h"
#include "D3D11Utils.h"

class D3D11Shader : public Shader {
public:
	D3D11Shader(ID3DBlob* inSource);
	~D3D11Shader();
	ID3DBlob* source;
};