#pragma once
#ifndef D3D11_RENDERING_INTERFACE_H
#define D3D11_RENDERING_INTERFACE_H

#include "D3D11Utils.h"
#include "RenderingInterface.h"
#include "D3D11VertexBuffer.h"

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

	VertexBuffer*	CreateVertexBuffer(unsigned int size, const void * data) final override;

private :
	ID3D11Texture2D*		CreateTexture2D(D3D11_TEXTURE2D_DESC* desc);

	ID3D11Device*			d3dDevice;
	ID3D11DeviceContext*	d3dImmediateContext;
	ID3D11RenderTargetView*	d3dRenderTargetView;
	ID3D11Texture2D*		depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	IDXGISwapChain*			swapChain;
	D3D_DRIVER_TYPE			driverType;

	HWND					windowHandle;

	bool					enable4xMsaa;
	UINT					xmsaaQuality;

	int						screenWidth;
	int						screenHeight;
};

#endif