#include "SpotLightComponent.h"

SpotLightComponent::SpotLightComponent() {}

SpotLightComponent::~SpotLightComponent() {
}

void SpotLightComponent::SetDirection(const XMFLOAT3 & directionVector) {
	direction = directionVector;
}

void SpotLightComponent::SetDirection(float x, float y, float z) {
	direction.x = x;
	direction.y = y;
	direction.z = z;
}

void SpotLightComponent::SetRange(float inRange) {
	range = inRange;
}

void SpotLightComponent::SetConeAngle(float angle) {
	coneAngle = angle;
}

void SpotLightComponent::SetPenumbraAngle(float angle) {
	penumbraAngle = angle;
}

XMFLOAT3 SpotLightComponent::GetDirection() const {
	return direction;
}

float SpotLightComponent::GetRange() const {
	return range;
}

float SpotLightComponent::GetConeAngle() const {
	return coneAngle;
}

float SpotLightComponent::GetPenumbraAngle() const {
	return penumbraAngle;
}

float SpotLightComponent::GetConeAngleRadians() const {
	return XMConvertToRadians(coneAngle);
}

float SpotLightComponent::GetPenumbraAngleRadians() const {
	return XMConvertToRadians(penumbraAngle);
}
