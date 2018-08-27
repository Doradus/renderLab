#pragma once
#include "RenderingResources.h"
#include "MathUtils.h"
#include "LightComponent.h"
#include "SpotLightComponent.h"
using namespace DirectX;
class ShadowInfo {
public:
	ShadowInfo();
	~ShadowInfo();

	void CreateShadowInfo(LightComponent* lightComponent);

	bool GetIsOmniDirectionalShadow() const;
	//bool GetIsCascadingShadow() const;
	//XMFLOAT4X4 GetShadowViewProjectionMatrix() const;
	std::vector<XMFLOAT4X4> GetShadowViewProjectionMatrices() const;

private:
	bool isOmniDirectionalShadow;
	bool isCascadingShadow;
	bool isDirectional;
	bool isSpot;
	unsigned int shadowId;
	XMFLOAT4X4 shadowViewProjectionMatrix;
	std::vector<XMFLOAT4X4> shadowViewProjectionMatrices;

};
