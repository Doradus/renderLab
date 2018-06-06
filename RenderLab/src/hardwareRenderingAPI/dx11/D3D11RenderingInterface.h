#pragma once
#ifndef D3D11_RENDERING_INTERFACE_H
#define D3D11_RENDERING_INTERFACE_H

#include "D3D11Utils.h"
#include "RenderingInterface.h"
#include "D3D11VertexBuffer.h"
#include "D3D11IndexBuffer.h"
#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "D3D11Resources.h"

class D3D11RenderingInterface : public RenderingInterface {
public :
	D3D11RenderingInterface(int screenWidth, int screenHeight, HWND mainWindow);
	virtual ~D3D11RenderingInterface();
	D3D11RenderingInterface(const D3D11RenderingInterface& that) = delete;

	void			InitRenderer() final override;

	void			CreateDepthAndStencilBuffer();
	void			CreateRenderTarget();

	bool			CreateDevice();
	bool			CreateSwapChain();

	VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) const final override;
	IndexBuffer*	CreateIndexBuffer(unsigned int size, const void * data) const final override;
	VertexShader*	CreateVertexShader(const unsigned char* shaderSource, size_t size) const final override;
	PixelShader*	CreatePixelShader(const unsigned char* shaderSource, size_t size) const final override;

	void			CreateInputLayout(const unsigned char* shaderSource, size_t size) final override;
	void			CreateConstantBuffer();
	void			ConstantBuffersMiddFrame(ObjectProperties objectProperties, MaterialResource material) const final override;
	void			ConstantBuffersFrameStart(PixelShaderPerFrameResource perFrameResources) const final override;
	void			StartFrame() const final override;
	void			Draw(RenderData* renderData, VertexShader* vertexShader, PixelShader* pixelShader) final override;
	void			EndFrame() const final override;

private :
	D3D11Texture2d*			CreateTexture2d(unsigned int width, unsigned int height, unsigned int numberOfMips, unsigned char format, unsigned int flags) const;
	ID3D11Device*			d3dDevice;
	ID3D11DeviceContext*	d3dImmediateContext;
	ID3D11RenderTargetView*	d3dRenderTargetView;
	D3D11Texture*			depthStencilBuffer;

	IDXGISwapChain*			swapChain;
	D3D_DRIVER_TYPE			driverType;

	ID3D11InputLayout*		inputLayout;
	ID3D11Buffer*			vertexConstantBuffers[NumVertexConstantBuffers];
	ID3D11Buffer*			pixelConstantBuffers[NumPixelConstantBuffers];

	HWND					windowHandle;

	bool					enable4xMsaa;
	UINT					xmsaaQuality;

	int						screenWidth;
	int						screenHeight;
};

#endif