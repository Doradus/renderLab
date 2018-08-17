#include "PointLightComponent.h"

PointLightComponent::PointLightComponent() {
	viewMatrix.push_back(XMFLOAT4X4());
	viewMatrix.push_back(XMFLOAT4X4());
	viewMatrix.push_back(XMFLOAT4X4());
	viewMatrix.push_back(XMFLOAT4X4());
	viewMatrix.push_back(XMFLOAT4X4());
	viewMatrix.push_back(XMFLOAT4X4());
}

PointLightComponent::~PointLightComponent() {

}

void PointLightComponent::SetRange(const float & inRange) {
	range = inRange;
}

void PointLightComponent::UpdateProjectionMatrix() {
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), 1.0, 1.0f, range);
	XMStoreFloat4x4(&projectionMatrix, projection);
}

void PointLightComponent::UpdateViewMatrix() {
	XMVECTOR lightPosition = XMLoadFloat3(&position);
	XMVECTOR upDirection = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));

	XMVECTOR left = XMLoadFloat3(&XMFLOAT3(range, 0.0f, 0.0f));
	XMVECTOR right = XMLoadFloat3(&XMFLOAT3(-range, 0.0f, 0.0f));
	XMVECTOR up = XMLoadFloat3(&XMFLOAT3(0.0f, range, 0.0f));
	XMVECTOR down = XMLoadFloat3(&XMFLOAT3(0.0f, -range, 0.0f));
	XMVECTOR front = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, range));
	XMVECTOR back = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, -range));

	XMMATRIX leftView = XMMatrixLookToLH(lightPosition, left, upDirection);
	XMStoreFloat4x4(&viewMatrix[0], leftView);

	XMMATRIX rightView = XMMatrixLookToLH(lightPosition, right, upDirection);
	XMStoreFloat4x4(&viewMatrix[1], rightView);

	XMMATRIX upView = XMMatrixLookToLH(lightPosition, up, upDirection);
	XMStoreFloat4x4(&viewMatrix[2], upView);

	XMMATRIX downView = XMMatrixLookToLH(lightPosition, down, upDirection);
	XMStoreFloat4x4(&viewMatrix[3], downView);

	XMMATRIX frontView = XMMatrixLookToLH(lightPosition, front, upDirection);
	XMStoreFloat4x4(&viewMatrix[4], frontView);

	XMMATRIX backView = XMMatrixLookToLH(lightPosition, back, upDirection);
	XMStoreFloat4x4(&viewMatrix[5], backView);
}

float PointLightComponent::GetRange() const {
	return range;
}

std::vector<XMFLOAT4X4> PointLightComponent::GetViewMatrix() const {
	return viewMatrix;
}

XMFLOAT4X4 PointLightComponent::GetProjectionMatrix() const {
	return projectionMatrix;
}
