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
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), 1.0f, 1.0f, range);
	XMStoreFloat4x4(&projectionMatrix, projection);
}

void PointLightComponent::UpdateViewMatrix() {
	XMVECTOR lightPosition = XMLoadFloat3(&position);
	XMVECTOR upDirection1 = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
	XMVECTOR upDirection2 = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, -1.0f));
	XMVECTOR upDirection3 = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 1.0f));

	XMVECTOR left = XMLoadFloat3(&XMFLOAT3(position.x + range,	position.y + 0.0f, position.z + 0.0f));
	XMVECTOR right = XMLoadFloat3(&XMFLOAT3(position.x + -range, position.y + 0.0f, position.z + 0.0f));
	XMVECTOR up = XMLoadFloat3(&XMFLOAT3(position.x + 0.0f, position.y + range, position.z + 0.0f));
	XMVECTOR down = XMLoadFloat3(&XMFLOAT3(position.x + 0.0f, position.y + -range, position.z + 0.0f));
	XMVECTOR front = XMLoadFloat3(&XMFLOAT3(position.x + 0.0f, position.y + 0.0f, position.z + range));
	XMVECTOR back = XMLoadFloat3(&XMFLOAT3(position.x + 0.0f, position.y + 0.0f, position.z + -range));

	XMMATRIX leftView = XMMatrixLookAtLH(lightPosition, left, upDirection1);
	XMStoreFloat4x4(&viewMatrix[0], leftView);

	XMMATRIX rightView = XMMatrixLookAtLH(lightPosition, right, upDirection1);
	XMStoreFloat4x4(&viewMatrix[1], rightView);

	XMMATRIX upView = XMMatrixLookAtLH(lightPosition, up, upDirection2);
	XMStoreFloat4x4(&viewMatrix[2], upView);

	XMMATRIX downView = XMMatrixLookAtLH(lightPosition, down, upDirection3);
	XMStoreFloat4x4(&viewMatrix[3], downView);

	XMMATRIX frontView = XMMatrixLookAtLH(lightPosition, front, upDirection1);
	XMStoreFloat4x4(&viewMatrix[4], frontView);

	XMMATRIX backView = XMMatrixLookAtLH(lightPosition, back, upDirection1);
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
