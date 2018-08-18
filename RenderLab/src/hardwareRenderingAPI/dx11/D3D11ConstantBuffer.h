#pragma once
#include "D3D11Utils.h"
#include "ConstantBuffer.h"

class D3D11ConstantBuffer : public ConstantBuffer {
public:
	D3D11ConstantBuffer(ID3D11Buffer* inResource);
	~D3D11ConstantBuffer();

public:
	ID3D11Buffer* resource;
};

