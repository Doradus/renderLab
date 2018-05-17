#pragma once
#include "PrimitiveFactory.h"
#include "Renderer.h"
#include "CameraComponent.h"
#include "StaticMesh.h"
#include "Timer.h"
#include "RenderingResources.h"

class RenderLab {
public:
	RenderLab(HWND windowHandle);
	~RenderLab();
	RenderLab(const RenderLab& that) = delete;
	RenderLab& operator = (const RenderLab&) = delete;

	bool		InitLab();
	void		Tick();
	void		PrepareStart();
	void		ShutDown();
	void		Draw();

private:
	bool CreateRenderer();
	void BuildGeometry();
	void InitShaders();
	void CreateCamera();
	void CreateMesh();
	void CreateWorld();
	void CreateLights();

	World*						world;
	Renderer*					renderer;
	HWND						windowHandle;

	VertexBuffer*				vertexBuffer;
	IndexBuffer*				indexBuffer;
	VertexShader*				vertextShader;
	PixelShader*				pixelShader;

	CameraComponent*			camera;
	StaticMesh*					mesh;
	DirectionalLightComponent*	light;
	Timer						timer;

	float						testRotation;
};