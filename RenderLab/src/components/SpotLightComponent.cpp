#include "SpotLightComponent.h"

SpotLightComponent::SpotLightComponent() {}

SpotLightComponent::~SpotLightComponent() {
}

void SpotLightComponent::UpdateViewMatrix() {
	XMVECTOR lightPosition = XMLoadFloat3(&position);
	XMVECTOR lightDirection = XMLoadFloat3(&direction);
	XMVECTOR up = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));

	XMMATRIX view = XMMatrixLookToLH(lightPosition, lightDirection, up);
	XMStoreFloat4x4(&viewMatrix, view);
}

void SpotLightComponent::UpdateProjectionMatrix() {
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(coneAngle + penumbraAngle), 1.0, 1.0f, range);
	XMStoreFloat4x4(&projectionMatrix, projection);
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

XMFLOAT4X4 SpotLightComponent::GetProjectionMatrix() const {
	return projectionMatrix;
}

XMFLOAT4X4 SpotLightComponent::GetViewMatrix() const {
	return viewMatrix;
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
