#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class RenderData {
public:
	RenderData();
	~RenderData();
	void			SetVertexBuffer(VertexBuffer* buffer);
	void			SetIndexBuffer(IndexBuffer* buffer);
	void			SetNumIndices(unsigned int numberOfIndices);

	VertexBuffer*	GetVertexBuffer() const;
	IndexBuffer*	GetIndexBuffer() const;
	unsigned int	GetNumIndices() const;

private:
	VertexBuffer*	vertexBuffer;
	IndexBuffer*	indexBuffer;
	unsigned int	numIndices;
};