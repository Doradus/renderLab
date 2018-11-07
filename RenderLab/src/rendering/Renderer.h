#pragma once
#include "RenderingInterface.h"
#include "D3D11RenderingInterface.h"
#include "StaticMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "World.h"
#include "ShadowInfo.h"
#include "Image.h"
#include "ResourceManager.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	void CreateHardwareRenderingInterface(int screenWidth, int screenHeight, HWND mainWindow);
	void AllocateShadowRenderTargets(World* world);
	void RenderWorld(World* world) const;
	void RenderPrimitive(const StaticMesh* mesh) const;

	VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const;
	IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const;
	VertexShader*	CreateVertexShader(const unsigned char* shaderSource, size_t size) const;
	PixelShader*	CreatePixelShader(const unsigned char* shaderSource, size_t size) const;

	void			CreateInputLayout(const unsigned char* shaderSource, size_t size) const;

private:
	void InitTextureResources();
	void InitShaders();
	void RenderProjectedOmniDirectionalShadow(World* world, ShadowInfo& shadowInfo) const;
	void RenderProjectedShadow(World* world, ShadowInfo& shadowInfo) const;
	void CreateConstantBuffers();
private:
	VertexShader*	shadowPassVS; 
	VertexShader*	omniDirectionalShadowPassVS;
	GeometryShader* omniDirectionalShadowPassGS;
	SamplerState*	samplerState;
	SamplerState*	omniDirectionalShadowSampler;
	SamplerState*	textureSampler;
	SamplerState*	normalMapSampler;

	ConstantBuffer* objectConstantBuffer;
	ConstantBuffer* shadowConstantBuffer;
	ConstantBuffer* pixelShaderPerFrameBuffer;
	ConstantBuffer* materialBuffer;
	ConstantBuffer* omniDirectionalShadowPassVSBuffer;
	ConstantBuffer* omniDirectionalShadowPassGSBuffer;
	ConstantBuffer* lightSpaceTransformBuffer;

	TextureRI* shadowMap;
	TextureRI* shadowMapCube;
	TextureRI* diffuseMap;
	TextureRI* normalMap;

	int screenWidth;
	int screenHeight;
};