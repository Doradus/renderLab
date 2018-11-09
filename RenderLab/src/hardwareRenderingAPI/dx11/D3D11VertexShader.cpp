#include "D3D11VertexShader.h"

D3D11VertexShader::D3D11VertexShader(char * inSrcCode) :
VertexShader(inSrcCode) {}

D3D11VertexShader::~D3D11VertexShader() {
	RELEASE(resource);
}