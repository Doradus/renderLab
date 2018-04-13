#include "D3D11VertexBuffer.h"

D3D11VertexBuffer::D3D11VertexBuffer(ID3D11Buffer* inResource) :
	resource(inResource)
{
}

D3D11VertexBuffer::~D3D11VertexBuffer() {
	RELEASE(resource);
}