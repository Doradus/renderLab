#pragma once
#ifndef RENDERING_INTERFACE_H
#define RENDERING_INTERFACE_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderingResources.h"
#include "RenderData.h"
#include "RenderUtils.h"
#include "SamplerState.h"
#include "RenderingInterfaceResources.h"
#include "GeometryShader.h"

class RenderingInterface {
public:
	RenderingInterface(); 
	virtual ~RenderingInterface();

	virtual void			InitRenderer() = 0;
	virtual VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const = 0;
	virtual IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const = 0;
	virtual ConstantBuffer*	CreateConstantBuffer(unsigned int size) const = 0;
	virtual VertexShader*	CreateVertexShader(const unsigned char* shaderSource, size_t size) const = 0;
	virtual PixelShader*	CreatePixelShader(const unsigned char* shaderSource, size_t size) const = 0;
	virtual GeometryShader*	CreateGeometryShader(const unsigned char* shaderSource, size_t size) const = 0;
	virtual SamplerState*	CreateSamplerState(const SamplerConfig& config) const = 0;
	virtual Texture2DRI*	CreateTexture2d(unsigned int width, unsigned int height, unsigned int arraySize, bool isCube, bool isTextureArray, unsigned int numberOfMips, unsigned char format, unsigned int flags, unsigned int samples, const void* resourceData) const = 0;
	virtual void			UpdateConstantBuffer(ConstantBuffer* buffer, void* data, unsigned int size) const = 0;
	virtual void			SetVSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const = 0;
	virtual void			SetGSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const = 0;
	virtual void			SetPSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const = 0;
	virtual void			SetViewPort(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const = 0;
	virtual void			SetRenderTarget(unsigned int numberOfRenderTargets, RenderTargetInfo* renderTarget, RenderTargetInfo* depthTarget) = 0;
	virtual void			BindBackBuffer() = 0;
	virtual void			SetVertexShader(VertexShader* shader) const = 0;
	virtual void			SetGeometryShader(GeometryShader* shader) const = 0;
	virtual void			SetPixelShader(PixelShader* shader) const = 0;
	virtual void			ClearActiveRenderTarget() const = 0;
	virtual void			SetShaderResources(TextureRI* shaderResource, unsigned int slot) const = 0;
	virtual void			SetSamplerState(SamplerState* samplerState, unsigned int slot) const = 0;
	virtual void			ClearShaderResource(unsigned int slot) const = 0;
	virtual void			SetShadowRasterState() const = 0;
	virtual void			SetDeafultRasterState() const = 0;

	// todo: refactor to shader state
	virtual void			CreateInputLayout(const unsigned char* shaderSource, size_t size) = 0;
	virtual void			ConstantBuffersFrameStart(PixelShaderPerFrameResource frameResources) const = 0;
	virtual void			UpdateShadowConstantBuffer(VertexShaderShadowResources shadowResources) const = 0;
	virtual void			ConstantBuffersMiddFrame(ObjectProperties objectProperties, MaterialResource material) const = 0;

	virtual void			StartFrame() const = 0;
	virtual void			Draw(RenderData* renderData, VertexShader* vertexShader, PixelShader* pixelShader) = 0;
	virtual void			EndFrame() const = 0;
};

extern RenderingInterface* renderingInterface;
#endif