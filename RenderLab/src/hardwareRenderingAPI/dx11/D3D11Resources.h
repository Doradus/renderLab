#pragma once
#include "D3D11Utils.h"

class D3D11Texture {
public:
	D3D11Texture(
		unsigned int inWidth,
		unsigned int inHeight,
		ID3D11DepthStencilView* inDepthStencileView,
		ID3D11ShaderResourceView* inShaderResourceView,
		ID3D11RenderTargetView*	inRenderTargetView
		) 
		: width (inWidth),
		height (inHeight),
		depthStencileView (inDepthStencileView),
		shaderResourceView (inShaderResourceView),
		renderTargetView (renderTargetView) {}


	~D3D11Texture() {};

protected:
	unsigned int width;
	unsigned int height;

	ID3D11DepthStencilView*		depthStencileView;
	ID3D11ShaderResourceView*	shaderResourceView;
	ID3D11RenderTargetView*		renderTargetView;
};