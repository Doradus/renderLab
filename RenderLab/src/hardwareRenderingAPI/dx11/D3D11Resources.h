 #pragma once
#include "D3D11Utils.h"
#include "RenderingInterfaceResources.h"

class D3D11Texture : virtual public TextureRI {
public:
	D3D11Texture(
		ID3D11DepthStencilView* inDepthStencileView,
		ID3D11ShaderResourceView* inShaderResourceView,
		std::vector<ID3D11RenderTargetView*> inRenderTargetViews
		) 
		: 
		depthStencileView (inDepthStencileView),
		shaderResourceView (inShaderResourceView),
		renderTargetViews (inRenderTargetViews) {}


	virtual ~D3D11Texture() {
		RELEASE(depthStencileView);
		RELEASE(shaderResourceView);

		for (ID3D11RenderTargetView* rtv : renderTargetViews) {
			RELEASE(rtv);
		}
	};

	virtual ID3D11DepthStencilView* GetDepthStencilView() const {
		return depthStencileView;
	}

	virtual ID3D11ShaderResourceView* GetShaderResourceView() const {
		return shaderResourceView;
	}

	virtual ID3D11RenderTargetView* GetRenderTargetViews(unsigned int arraySlize) const {
		return renderTargetViews[arraySlize];
	}

protected:
	ID3D11DepthStencilView*					depthStencileView;
	ID3D11ShaderResourceView*				shaderResourceView;
	std::vector<ID3D11RenderTargetView*>	renderTargetViews;
};


class D3D11Texture2d : public D3D11Texture, public Texture2DRI {
public:
	D3D11Texture2d(
		ID3D11DepthStencilView* inDepthStencileView,
		ID3D11ShaderResourceView* inShaderResourceView,
		std::vector<ID3D11RenderTargetView*>	inRenderTargetView
	)
		: D3D11Texture(
			inDepthStencileView,
			inShaderResourceView,
			inRenderTargetView
		) {}


	~D3D11Texture2d() {};
};