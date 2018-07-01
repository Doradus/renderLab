 #pragma once
#include "D3D11Utils.h"
#include "RenderingInterfaceResources.h"

class D3D11Texture : public TextureRI {
public:
	D3D11Texture(
		ID3D11DepthStencilView* inDepthStencileView,
		ID3D11ShaderResourceView* inShaderResourceView,
		ID3D11RenderTargetView*	inRenderTargetView
		) 
		: 
		depthStencileView (inDepthStencileView),
		shaderResourceView (inShaderResourceView),
		renderTargetView (inRenderTargetView) {}


	virtual ~D3D11Texture() {
		RELEASE(depthStencileView);
		RELEASE(shaderResourceView);
		RELEASE(renderTargetView);
	};

	virtual ID3D11DepthStencilView* GetDepthStencilView() const {
		return depthStencileView;
	}

	virtual ID3D11ShaderResourceView* GetShaderResourceView() const {
		return shaderResourceView;
	}

	virtual ID3D11RenderTargetView* GetRenderTargetView() const {
		return renderTargetView;
	}

protected:
	ID3D11DepthStencilView*		depthStencileView;
	ID3D11ShaderResourceView*	shaderResourceView;
	ID3D11RenderTargetView*		renderTargetView;
};


class D3D11Texture2d : public D3D11Texture, public Texture2DRI {
public:
	D3D11Texture2d(
		ID3D11DepthStencilView* inDepthStencileView,
		ID3D11ShaderResourceView* inShaderResourceView,
		ID3D11RenderTargetView*	inRenderTargetView
	)
		: D3D11Texture(
			inDepthStencileView,
			inShaderResourceView,
			inRenderTargetView
		) {}


	~D3D11Texture2d() {};
};