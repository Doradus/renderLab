#pragma once
#include "LightComponent.h"
#include "Texture2D.h"

class DirectionalLightComponent : public LightComponent  {
public:
	DirectionalLightComponent();
	~DirectionalLightComponent();

	void		SetDirection(XMFLOAT3 inDirection);
	void		SetDirection(float x, float y, float z);
	XMFLOAT3	GetDirection() const;
	Texture2D*	GetShadowMap() const;

private:
	XMFLOAT3 direction;
	Texture2D* shadowMap;
};