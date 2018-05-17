#pragma once
#include "RenderingResources.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SceneComponent.h"
#include "VertexShader.h"
#include "PixelShader.h"

class StaticMesh : public SceneComponent {
public:
	StaticMesh();
	~StaticMesh();

	VertexBuffer*	GetVertexBuffer() const;
	IndexBuffer*	GetIndexBuffer() const;
	VertexShader*	GetVertexShader() const;
	PixelShader*	GetPixelShader() const;

	void			SetVertexBuffer(VertexBuffer* buffer);
	void			SetIndexBuffer(IndexBuffer* buffer);
	void			SetVertexShader(VertexShader* shader);
	void			SetPixelShader(PixelShader* shader);

private:
	VertexBuffer*	vertexBuffer;
	IndexBuffer*	indexBuffer;

	VertexShader*	vertexShader;
	PixelShader*	pixelShader;
};