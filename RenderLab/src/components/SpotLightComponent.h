#pragma once
#include "LightComponent.h"
class SpotLightComponent : public LightComponent {
public:
	SpotLightComponent();
	~SpotLightComponent();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void SetDirection(const XMFLOAT3& directionVector);
	void SetDirection(float x, float y, float z);
	void SetRange(float range);
	void SetConeAngle(float angle);
	void SetPenumbraAngle(float angle);

	XMFLOAT3 GetDirection() const;
	XMFLOAT4X4 GetProjectionMatrix() const;
	XMFLOAT4X4 GetViewMatrix() const;
	float GetRange() const;
	float GetConeAngle() const;
	float GetPenumbraAngle() const;
	float GetConeAngleRadians() const;
	float GetPenumbraAngleRadians() const;

private:
	XMFLOAT3		direction;
	XMFLOAT4X4		viewMatrix;
	XMFLOAT4X4		projectionMatrix;
	float			range;
	float			coneAngle;
	float			penumbraAngle;
};
