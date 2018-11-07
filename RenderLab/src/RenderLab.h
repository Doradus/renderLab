#pragma once
#include "PrimitiveFactory.h"
#include "Renderer.h"
#include "CameraComponent.h"
#include "StaticMesh.h"
#include "Timer.h"
#include "RenderingResources.h"
#include "FileSystem.h"
#include "ResourceManager.h"

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
	VertexBuffer*				planeVertexBuffer;
	IndexBuffer*				planeIndexBuffer;
	VertexBuffer*				sphereVertexBuffer;
	IndexBuffer*				sphereIndexBuffer;

	VertexShader*				vertextShader;
	PixelShader*				pixelShader;

	CameraComponent*			camera;
	StaticMesh*					box;
	StaticMesh*					box2;
	StaticMesh*					plane;
	StaticMesh*					sphere;
	DirectionalLightComponent*	light;
	PointLightComponent*		pointLight;
	SpotLightComponent*			spotLight;
	SpotLightComponent*			spotLight2;
	Timer						timer;

	//material
	Material*					boxMaterial;
	Material*					box2Material;
	Material*					sphereMaterial;
	Material*					planeMaterial;

	float						testRotation;
};