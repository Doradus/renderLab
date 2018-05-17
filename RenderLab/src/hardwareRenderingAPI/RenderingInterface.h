#pragma once
#ifndef RENDERING_INTERFACE_H
#define RENDERING_INTERFACE_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderingResources.h"

class RenderingInterface {
public:
	RenderingInterface(); 
	virtual ~RenderingInterface();

	virtual void			InitRenderer() = 0;
	virtual VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const = 0;
	virtual IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const = 0;
	virtual VertexShader*	CreateVertexShader(const unsigned char* shaderSource, size_t size) const = 0;
	virtual PixelShader*	CreatePixelShader(const unsigned char* shaderSource, size_t size) const = 0;

	// todo: refactor to shader state
	virtual void			CreateInputLayout(const unsigned char* shaderSource, size_t size) = 0;
	virtual void			CreateConstantBuffer() = 0;
	virtual void			ConstantBuffersFrameStart(DirectionalLightResource light) const = 0;
	virtual void			ConstantBuffersMiddFrame(ObjectProperties objectProperties) const = 0;

	virtual void			Draw(VertexBuffer* vertices, IndexBuffer* indices, VertexShader* vertexShader, PixelShader* pixelShader) = 0;
};

#endif