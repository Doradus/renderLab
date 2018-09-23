#pragma once
#include "RenderingResources.h"
#include "MathUtils.h"
#include "LightComponent.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"
#include "DirectionalLightComponent.h"
using namespace DirectX;
class ShadowInfo {
public:
	ShadowInfo();
	~ShadowInfo();

	void CreateShadowInfo(LightComponent* lightComponent);

	bool GetIsOmniDirectionalShadow() const;
	bool GetIsProjectedShadow() const;
	//bool GetIsCascadingShadow() const;
	XMFLOAT4X4 GetShadowViewProjectionMatrix() const;
	XMFLOAT4X4 GetShadowViewMatrix() const;
	unsigned int GetShadowId() const;
	std::vector<XMFLOAT4X4> GetShadowViewProjectionMatrices() const;

private:
	bool isOmniDirectionalShadow;
	bool isCascadingShadow;
	bool isDirectional;
	bool isProjectedShadow;
	unsigned int shadowId;
	XMFLOAT4X4 shadowViewProjectionMatrix;
	XMFLOAT4X4 shadowViewMatrix;
	std::vector<XMFLOAT4X4> shadowViewProjectionMatrices;

};
