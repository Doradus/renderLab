#include "D3D11GeometryShader.h"

D3D11GeometryShader::D3D11GeometryShader(ID3D11GeometryShader* inResource) :
	resource(inResource)
{
}

D3D11GeometryShader::~D3D11GeometryShader() {
	RELEASE(resource);
}
