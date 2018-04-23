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

	RenderingInterface* renderingInterface;
	HWND windowHandle;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
};