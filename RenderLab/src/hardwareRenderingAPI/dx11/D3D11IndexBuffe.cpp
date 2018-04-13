#include "D3D11IndexBuffer.h"

D3D11IndexBuffer::D3D11IndexBuffer(ID3D11Buffer* inResource) :
	resource(inResource)
{
}

D3D11IndexBuffer::~D3D11IndexBuffer() {
	RELEASE(resource);
}