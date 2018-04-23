#include "D3D11PixelShader.h"

D3D11PixelShader::D3D11PixelShader() {}

D3D11PixelShader::~D3D11PixelShader() {
	RELEASE(resource);
}