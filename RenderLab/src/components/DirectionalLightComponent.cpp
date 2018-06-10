#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent() {
}

DirectionalLightComponent::~DirectionalLightComponent() {
}

void DirectionalLightComponent::SetDirection(XMFLOAT3 inDirection) {
	direction = inDirection;
}

void DirectionalLightComponent::SetDirection(float x, float y, float z) {
	direction.x = x;
	direction.y = y;
	direction.z = z;
}

XMFLOAT3 DirectionalLightComponent::GetDirection() const {
	return direction;
}

Texture2D * DirectionalLightComponent::GetShadowMap() const {
	return shadowMap;
}
