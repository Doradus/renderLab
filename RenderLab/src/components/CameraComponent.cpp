#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	upDirection(0.0f, 1.0f, 0.0f)
{
	position = { 0.0f, 10.0f, 0.0f };
}

CameraComponent::CameraComponent(float aspectRatio, float nearPlane, float farPlane, float fieldOfView) :
		aspectRatio (aspectRatio),
		nearPlane (nearPlane),
		farPlane (farPlane),
		fieldOfView (fieldOfView),
		upDirection(0.0f, 1.0f, 0.0f) {
	
	position = { 0.0f, 10.0f, 0.0f };
	UpdateProjection();
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::UpdateProjection() {
	XMMATRIX projection = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);
	XMStoreFloat4x4(&projectionMatrix, projection);
}

void CameraComponent::UpdateView() {
	XMVECTOR cameraPosition = XMLoadFloat3(&position);
	XMVECTOR target = XMLoadFloat3(&cameraTarget);
	XMVECTOR up = XMLoadFloat3(&upDirection);

	XMMATRIX view = XMMatrixLookAtLH(cameraPosition, target, up);
	XMStoreFloat4x4(&cameraView, view);
}

void CameraComponent::SetAspectratio(float ratio) {
	aspectRatio = ratio;
}

void CameraComponent::SetFieldOfView(float fov) {
	fieldOfView = fov;
}

void CameraComponent::SetNearPlane(float value) {
	nearPlane = value;
}

void CameraComponent::SetFarPlane(float value) {
	farPlane = value;
}

void CameraComponent::SetCameraTarget(XMFLOAT3 target) {
	cameraTarget = target;
}

void CameraComponent::SetCameraTarget(float x, float y, float z) {
	cameraTarget.x = x;
	cameraTarget.y = y;
	cameraTarget.z = z;
}

XMFLOAT4X4 CameraComponent::GetProjection() const {
	return projectionMatrix;
}

XMFLOAT4X4 CameraComponent::GetCameraView() const {
	return cameraView;
}

XMFLOAT3 CameraComponent::GetCameraTarget() const {
	return cameraTarget;
}



