#pragma once
#pragma once
#include "IndexBuffer.h"
#include "D3D11Utils.h"

class D3D11IndexBuffer : public IndexBuffer {
public:
	D3D11IndexBuffer(ID3D11Buffer* inResource);
	~D3D11IndexBuffer();
	ID3D11Buffer* resource;
};