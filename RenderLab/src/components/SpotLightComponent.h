#pragma once
#include "LightComponent.h"
class SpotLightComponent : public LightComponent {
public:
	SpotLightComponent();
	~SpotLightComponent();

	void SetDirection(const XMFLOAT3& directionVector);
	void SetDirection(float x, float y, float z);
	void SetRange(float range);
	void SetConeAngle(float angle);
	void SetPenumbraAngle(float angle);

	XMFLOAT3 GetDirection() const;
	float GetRange() const;
	float GetConeAngle() const;
	float GetPenumbraAngle() const;
	float GetConeAngleRadians() const;
	float GetPenumbraAngleRadians() const;

private:
	XMFLOAT3 direction;
	float range;
	float coneAngle;
	float penumbraAngle;
};
