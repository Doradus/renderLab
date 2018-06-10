#pragma once
#include "D3D11Utils.h"

enum ShaderTypes {
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER,
	SHADER_TYPES
};

class RenderStateCache {
public:
	RenderStateCache () :
	d3dImmediateContext(nullptr),
	currentInputLayout (nullptr), 
	currentVertexShader(nullptr),
	currentPixelShader(nullptr)
	{};
	~RenderStateCache () {};

	void SetContext(ID3D11DeviceContext* inContext) {
		d3dImmediateContext = inContext;
	}

	void SetInputLayout(ID3D11InputLayout* inputLayout) {
		if (inputLayout != currentInputLayout) {
			currentInputLayout = inputLayout;
			d3dImmediateContext->IASetInputLayout(inputLayout);
		}
	}

	void SetVertexShader(ID3D11VertexShader* shader) {
		if (shader != currentVertexShader) {
			currentVertexShader = shader;
			d3dImmediateContext->VSSetShader(shader, nullptr, 0);
		}
	}

	void SetPixelShader(ID3D11PixelShader* shader) {
		if (shader != currentPixelShader) {
			currentPixelShader = shader;
			d3dImmediateContext->PSSetShader(shader, nullptr, 0);
		}
	}

	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
		if (currentTopology != topology) {
			currentTopology = topology;
			d3dImmediateContext->IASetPrimitiveTopology(topology);
		}
	}

	void SetShaderResourceView(ID3D11ShaderResourceView* srv, ShaderTypes type, unsigned int slot) {
		if (currentShaderResourceView[type][slot] != srv) {
			currentShaderResourceView[type][slot] = srv;

			switch (type) {
				case VERTEX_SHADER :
					d3dImmediateContext->VSSetShaderResources(slot, 1, &srv);
					break;
				case HULL_SHADER :
					d3dImmediateContext->HSSetShaderResources(slot, 1, &srv);
					break;
				case DOMAIN_SHADER :
					d3dImmediateContext->DSSetShaderResources(slot, 1, &srv);
					break;
				case GEOMETRY_SHADER :
					d3dImmediateContext->GSSetShaderResources(slot, 1, &srv);
					break;
				case PIXEL_SHADER :
					d3dImmediateContext->PSSetShaderResources(slot, 1, &srv);
					break;
				case COMPUTE_SHADER :
					d3dImmediateContext->CSSetShaderResources(slot, 1, &srv);
					break;
				default:
					break;
			}
		}
	}

	void SetSamplerState(ID3D11SamplerState* sampler, ShaderTypes type, unsigned int slot) {
		if (currentSamplerState[type][slot] != sampler) {
			switch (type) {
			case VERTEX_SHADER:
				d3dImmediateContext->VSSetSamplers(slot, 1, &sampler);
				break;
			case HULL_SHADER:
				d3dImmediateContext->HSSetSamplers(slot, 1, &sampler);
				break;
			case DOMAIN_SHADER:
				d3dImmediateContext->DSSetSamplers(slot, 1, &sampler);
				break;
			case GEOMETRY_SHADER:
				d3dImmediateContext->GSSetSamplers(slot, 1, &sampler);
				break;
			case PIXEL_SHADER:
				d3dImmediateContext->PSSetSamplers(slot, 1, &sampler);
				break;
			case COMPUTE_SHADER:
				d3dImmediateContext->CSSetSamplers(slot, 1, &sampler);
				break;
			default:
				break;
			}
		}
	}

private:
	ID3D11DeviceContext*		d3dImmediateContext;
	ID3D11InputLayout*			currentInputLayout;
	ID3D11VertexShader*			currentVertexShader;
	ID3D11PixelShader*			currentPixelShader;
	ID3D11ShaderResourceView*	currentShaderResourceView[SHADER_TYPES][D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	ID3D11SamplerState*			currentSamplerState[SHADER_TYPES][D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
	D3D11_PRIMITIVE_TOPOLOGY	currentTopology;
};