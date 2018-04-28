#pragma once
#include "PrimitiveFactory.h"
#include "Renderer.h"
#include "CameraComponent.h"
#include "StaticMesh.h"

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
	void CreateCamera();
	void CreateMesh();
	void Draw();

	Renderer*			renderer;
	HWND				windowHandle;

	VertexBuffer*		vertexBuffer;
	IndexBuffer*		indexBuffer;
	VertexShader*		vertextShader;
	PixelShader*		pixelShader;

	CameraComponent*	camera;
	StaticMesh*			mesh;
};