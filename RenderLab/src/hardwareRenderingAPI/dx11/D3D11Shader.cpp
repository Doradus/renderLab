#include "D3D11Shader.h"

D3D11Shader::D3D11Shader(ID3DBlob* inSource) : 
	source(inSource)
{}

D3D11Shader::~D3D11Shader() {}