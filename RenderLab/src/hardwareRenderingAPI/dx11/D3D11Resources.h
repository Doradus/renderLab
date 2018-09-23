 #pragma once
#include "D3D11Utils.h"
#include "RenderingInterfaceResources.h"

class D3D11Texture : virtual public TextureRI {
public:
	D3D11Texture(
		std::vector<ID3D11DepthStencilView*> inDepthStencileViews,
		ID3D11ShaderResourceView* inShaderResourceView,
		std::vector<ID3D11RenderTargetView*> inRenderTargetViews
		) 
		: 
		depthStencileViews (inDepthStencileViews),
		shaderResourceView (inShaderResourceView),
		renderTargetViews (inRenderTargetViews) {}


	virtual ~D3D11Texture() {
		RELEASE(shaderResourceView);

		for (ID3D11DepthStencilView* dsv : depthStencileViews) {
			RELEASE(dsv);
		}

		for (ID3D11RenderTargetView* rtv : renderTargetViews) {
			RELEASE(rtv);
		}
	};

	virtual ID3D11DepthStencilView* GetDepthStencilView(unsigned int arraySlize) const {
		return depthStencileViews[arraySlize];
	}

	virtual ID3D11ShaderResourceView* GetShaderResourceView() const {
		return shaderResourceView;
	}

	virtual ID3D11RenderTargetView* GetRenderTargetViews(unsigned int arraySlize) const {
		return renderTargetViews[arraySlize];
	}

protected:
	std::vector<ID3D11DepthStencilView*>	depthStencileViews;
	ID3D11ShaderResourceView*				shaderResourceView;
	std::vector<ID3D11RenderTargetView*>	renderTargetViews;
};


class D3D11Texture2d : public D3D11Texture, public Texture2DRI {
public:
	D3D11Texture2d(
		std::vector<ID3D11DepthStencilView*> inDepthStencileViews,
		ID3D11ShaderResourceView* inShaderResourceView,
		std::vector<ID3D11RenderTargetView*>	inRenderTargetView
	)
		: D3D11Texture(
			inDepthStencileViews,
			inShaderResourceView,
			inRenderTargetView
		) {}


	~D3D11Texture2d() {};
};