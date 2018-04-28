#pragma once
#include "RenderingInterface.h"
#include "D3D11RenderingInterface.h"
#include "StaticMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	void CreateHardwareRenderingInterface(int screenWidth, int screenHeight, HWND mainWindow);
	void RenderPrimitive(StaticMesh* staticMesh) const;

	VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const;
	IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const;
	VertexShader*	CreateVertexShader(const unsigned char* shaderSource, size_t size) const;
	PixelShader*	CreatePixelShader(const unsigned char* shaderSource, size_t size) const;

	void			CreateInputLayout(const unsigned char* shaderSource, size_t size) const;
	void			CreateConstantBuffer() const;
	void			UpdateConstantBuffer(XMFLOAT4X4 matrix) const;

private:
	RenderingInterface* renderingInterface;
};