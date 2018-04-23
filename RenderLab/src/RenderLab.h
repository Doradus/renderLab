#pragma once

#include "RenderingInterface.h"
#include "D3D11RenderingInterface.h"
#include "PrimitiveFactory.h"

class RenderLab {
public:
	RenderLab(HWND windowHandle);
	~RenderLab();
	RenderLab(const RenderLab& that) = delete;
	RenderLab& operator = (const RenderLab&) = delete;

	bool		InitLab();
	void		ShutDown();

private:
	bool CreateRenderingInterface();
	void BuildGeometry();
	void InitShaders();
	void Draw();

	RenderingInterface* renderingInterface;
	HWND windowHandle;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	VertexShader* vertextShader;
	PixelShader* pixelShader;

	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
};