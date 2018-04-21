#include "D3D11VertexShader.h"

D3D11VertexShader::D3D11VertexShader() :
	resource(nullptr) {}

D3D11VertexShader::~D3D11VertexShader() {
	RELEASE(resource);
}