#pragma once
#include "LightComponent.h"

class PointLightComponent : public LightComponent {
public:
	PointLightComponent();
	~PointLightComponent();

	void SetRange(const float& inRange);

	float GetRange() const;

private:
	float range;
};