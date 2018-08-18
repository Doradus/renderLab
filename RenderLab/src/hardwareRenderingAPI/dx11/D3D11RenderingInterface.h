#pragma once
#ifndef D3D11_RENDERING_INTERFACE_H
#define D3D11_RENDERING_INTERFACE_H

#include "D3D11Utils.h"
#include "RenderingInterface.h"
#include "D3D11VertexBuffer.h"
#include "D3D11IndexBuffer.h"
#include "D3D11ConstantBuffer.h"
#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "D3D11GeometryShader.h"
#include "D3D11Resources.h"
#include "RenderState.h"
#include "D3D11SamplerState.h"

class D3D11RenderingInterface : public RenderingInterface {
public :
	D3D11RenderingInterface(int screenWidth, int screenHeight, HWND mainWindow);
	virtual ~D3D11RenderingInterface();
	D3D11RenderingInterface(const D3D11RenderingInterface& that) = delete;

	void			InitRenderer() final override;

	bool			CreateDevice();
	bool			CreateSwapChain();

	VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const final override;
	IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const final override;
	ConstantBuffer*	CreateConstantBuffer(unsigned int size) const final override;
	VertexShader*	CreateVertexShader(const unsigned char* shaderSource, size_t size) const final override;
	PixelShader*	CreatePixelShader(const unsigned char* shaderSource, size_t size) const final override;
	GeometryShader*	CreateGeometryShader(const unsigned char* shaderSource, size_t size) const final override;
	SamplerState*	CreateSamplerState(const SamplerConfig& config) const final override;
	Texture2DRI*	CreateTexture2d(unsigned int width, unsigned int height, unsigned int arraySize, bool isCube, unsigned int numberOfMips, unsigned char format, unsigned int flags, unsigned int samples) const final override;
	void			UpdateConstantBuffer(ConstantBuffer* buffer, void* data, unsigned int size) const;
	void			SetVSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const final override;
	void			SetGSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const final override;
	void			SetPSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const final override;
	void			SetViewPort(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
	void			SetRenderTarget(TextureRI* renderTarget, TextureRI* depthTarget) final override;
	void			BindBackBuffer() final override;
	void			SetVertexShader(VertexShader* shader) const;
	void			SetGeometryShader(GeometryShader* shader) const;
	void			SetPixelShader(PixelShader* shader) const;
	void			ClearActiveRenderTarget() const;
	void			SetShaderResources(TextureRI* shaderResource) const;
	void			SetSamplerState(SamplerState* samplerState) const;
	void			ClearShaderResource() const;
	void			SetShadowRasterState() const;
	void			SetDeafultRasterState() const;

	void			CreateInputLayout(const unsigned char* shaderSource, size_t size) final override;
	void			ConstantBuffersMiddFrame(ObjectProperties objectProperties, MaterialResource material) const final override;
	void			UpdateShadowConstantBuffer(VertexShaderShadowResources shadowResources) const final override;
	void			ConstantBuffersFrameStart(PixelShaderPerFrameResource perFrameResources) const final override;
	void			StartFrame() const final override;
	void			Draw(RenderData* renderData, VertexShader* vertexShader, PixelShader* pixelShader) final override;
	void			EndFrame() const final override;

private :
	D3D11Texture2d*			CreateD3D11Texture2d(unsigned int width, unsigned int height, unsigned int arraySize, bool isCube, unsigned int numberOfMips, unsigned char format, unsigned int flags, unsigned int samples) const;
	//D3D11Texture2d*			CreateTexture2dFromResource(ID3D11Texture2D* inResource, unsigned char format, unsigned int flags) const;
	D3D11Texture2d*			CreateBackBuffer() const;
	D3D11Texture2d*			CreateDepthAndStencilBuffer();
	void					InitDefaultRenderTargets();
	ID3D11Device*			d3dDevice;
	ID3D11DeviceContext*	d3dImmediateContext;

	D3D11Texture*			defaultRenderTargetView;
	D3D11Texture*			defaultDepthStencilBuffer;

	D3D11Texture*			activeRenderTargetView;
	D3D11Texture*			activeDepthStencilBuffer;

	IDXGISwapChain*			swapChain;
	D3D_DRIVER_TYPE			driverType;

	ID3D11InputLayout*		inputLayout;
	ID3D11Buffer*			vertexConstantBuffers[NumVertexConstantBuffers];
	ID3D11Buffer*			pixelConstantBuffers[NumPixelConstantBuffers];

	HWND					windowHandle;
	RenderStateCache*		stateCache;

	bool					enable4xMsaa;
	UINT					xmsaaQuality;

	int						screenWidth;
	int						screenHeight;

	D3D11_TEXTURE_ADDRESS_MODE	GetAddressMode(AddressModes mode) const;
	D3D11_COMPARISON_FUNC		GetSamplerCompareFunction(SamplerCompareFunction compare) const;
	DXGI_FORMAT					GetDepthStencilFormat(DXGI_FORMAT inFormat) const;
	DXGI_FORMAT					GetShaderResourceFormat(DXGI_FORMAT inFormat) const;

	ID3D11RasterizerState*		shadowState;
	ID3D11RasterizerState*		defaultState;
	void						CreateRasterStates();
};

#endif