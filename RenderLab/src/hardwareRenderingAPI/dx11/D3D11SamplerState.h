#pragma once
#include "SamplerState.h"
#include "D3D11Utils.h"

class D3D11SamplerState : public SamplerState {
public:
	D3D11SamplerState() {};
	~D3D11SamplerState() {
		RELEASE(samplerState);
	};

	ID3D11SamplerState* samplerState;
};
