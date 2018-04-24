#pragma once
#include "RenderUtils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SceneComponent.h"

class StaticMesh : public SceneComponent {
public:
	StaticMesh();
	~StaticMesh();


	VertexBuffer*	GetVertexBuffer() const;
	IndexBuffer*	GetIndexBuffer() const;
	void			SetVertexBuffer(VertexBuffer* buffer);
	void			SetIndexBuffer(IndexBuffer* buffer);

private:
	VertexBuffer*	vertexBuffer;
	IndexBuffer*	indexBuffer;
};