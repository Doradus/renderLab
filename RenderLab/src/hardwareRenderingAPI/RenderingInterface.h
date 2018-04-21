#pragma once
#ifndef RENDERING_INTERFACE_H
#define RENDERING_INTERFACE_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class RenderingInterface {
public:
	RenderingInterface(); 
	virtual ~RenderingInterface();

	virtual void			InitRenderer() = 0;
	virtual VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const = 0;
	virtual IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const = 0;
	virtual VertexShader*	CreateVertexShader(const unsigned char* shaderSource, size_t size) const = 0;
	virtual PixelShader*	CreatePixelShader(const void* shaderSource) const = 0;
};

#endif