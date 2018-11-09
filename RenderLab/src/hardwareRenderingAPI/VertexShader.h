#pragma once
#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H
#include "Shader.h"

class VertexShader : public Shader {
public:
	VertexShader(char* inSrcCode);
	virtual ~VertexShader();
};

#endif