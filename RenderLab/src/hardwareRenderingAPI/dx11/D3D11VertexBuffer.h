#pragma once
#include "VertexBuffer.h"
#include "D3D11Utils.h"

class D3D11VertexBuffer : public VertexBuffer {
public:
	D3D11VertexBuffer(ID3D11Buffer* inResource);
	~D3D11VertexBuffer();
	ID3D11Buffer* resource;
};