#include "SceneComponent.h"

SceneComponent::SceneComponent() {
	isDirty = false;
	SetPosition(0, 0, 0);
	SetScale(1, 1, 1);
	SetRotation(0, 0, 0);
}

SceneComponent::~SceneComponent()
{
}

void SceneComponent::SetPosition(XMFLOAT3 inPosition) {
	position = inPosition;
	isDirty = true;
}

void SceneComponent::SetPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
	isDirty = true;
}

void SceneComponent::SetScale(XMFLOAT3 inScale) {
	scale = inScale;
	isDirty = true;
}

void SceneComponent::SetScale(float inScale) {
	scale.x = inScale;
	scale.y = inScale;
	scale.z = inScale;
	isDirty = true;
}

void SceneComponent::SetScale(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
	isDirty = true;
}

void SceneComponent::SetRotation(XMFLOAT3 inRotation) {
	rotation = inRotation;
	isDirty = true;
}

void SceneComponent::SetRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	isDirty = true;
}

void SceneComponent::Move(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
	isDirty = true;
}

void SceneComponent::UpdateTransform() {
	if (isDirty) {
		XMMATRIX scalingMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

		float rotateX = XMConvertToRadians(rotation.x);
		float rotateY = XMConvertToRadians(rotation.y);
		float rotateZ = XMConvertToRadians(rotation.z);

		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotateX, rotateY, rotateZ);
		XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

		XMMATRIX world = scalingMatrix * rotationMatrix * translationMatrix;
		XMStoreFloat4x4(&worldMatrix, world);

		isDirty = false;
	}
}

XMFLOAT4X4 SceneComponent::GetWorld() const {
	return worldMatrix;
}

