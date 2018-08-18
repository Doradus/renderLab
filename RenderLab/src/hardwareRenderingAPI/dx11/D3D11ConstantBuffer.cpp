#include "D3D11ConstantBuffer.h"

D3D11ConstantBuffer::D3D11ConstantBuffer(ID3D11Buffer * inResource) :
resource(inResource) 
{}

D3D11ConstantBuffer::~D3D11ConstantBuffer() {
	RELEASE(resource);
}
