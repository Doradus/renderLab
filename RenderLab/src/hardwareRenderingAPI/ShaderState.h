#pragma once
#include "VertexShader.h"
#include "PixelShader.h"

class ShaderState {
public:
	ShaderState(VertexShader* inVertexShader, PixelShader*  inPixelShader);
	~ShaderState();


private:
	VertexShader* vertexShader;
	PixelShader*  pixelShader;
};