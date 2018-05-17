#pragma once
#include "LightComponent.h"

class DirectionalLightComponent : public LightComponent  {
public:
	DirectionalLightComponent();
	~DirectionalLightComponent();

	void		SetDirection(XMFLOAT3 inDirection);
	void		SetDirection(float x, float y, float z);
	XMFLOAT3	GetDirection() const;

private:
	XMFLOAT3 direction;
};