#pragma once
#ifndef RENDERING_INTERFACE_H
#define RENDERING_INTERFACE_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class RenderingInterface {
public:
	RenderingInterface(); 
	virtual ~RenderingInterface();

	virtual void			InitRenderer() = 0;
	virtual VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const = 0;
	virtual IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const = 0;
};

#endif