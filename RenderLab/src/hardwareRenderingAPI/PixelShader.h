#pragma once
#pragma once
#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H
#include "Shader.h"

class PixelShader : public Shader {
public:
	PixelShader(char * inSrcCod);
	virtual ~PixelShader();
};

#endif