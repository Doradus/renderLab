#include "ShadowInfo.h"

ShadowInfo::ShadowInfo() : 
	isCascadingShadow (false),
	isOmniDirectionalShadow(false),
	isProjectedShadow(false),
	isDirectional(false),
	shadowId(0)
{
	shadowViewProjectionMatrices = {};
}

ShadowInfo::~ShadowInfo() {
}

void ShadowInfo::CreateShadowInfo(LightComponent * lightComponent) {
	bool isSpot = false;

	switch (lightComponent->GetLightType()) {
		case SPOT_LIGHT : 
			isSpot = true;
			isProjectedShadow = true;
			break;
		case POINT_LIGHT :
			isOmniDirectionalShadow = true;
			break;
		case DIRECTIONAL_LIGHT :
			isDirectional = true;
			isProjectedShadow = true;
			break;
		default:
			break;
	}

	if (isSpot) {
		const SpotLightComponent* spotLight = static_cast<SpotLightComponent*>(lightComponent);
		const XMFLOAT3 position = spotLight->GetPosition();
		const XMFLOAT3 direction = spotLight->GetDirection();

		const float coneAngle = spotLight->GetConeAngle();
		const float penumbraAngle = spotLight->GetPenumbraAngle();
		const float range = spotLight->GetRange();

		XMVECTOR lightPosition = XMLoadFloat3(&position);
		XMVECTOR lightDirection = XMLoadFloat3(&direction);
		XMVECTOR focus = { position.x + (direction.x * range), position.y + (direction.y * range), position.z + (direction.z * range) };
		XMVECTOR up = { 0.0f, 1.0f, 0.0f };

		XMMATRIX view = XMMatrixLookAtLH(lightPosition, focus, up);

		XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians((coneAngle + penumbraAngle) * 2), 1.0, 0.5f, 500);

		XMStoreFloat4x4(&shadowViewMatrix, view);
		XMStoreFloat4x4(&shadowViewProjectionMatrix, projection);
	}

	if (isOmniDirectionalShadow) {
		const PointLightComponent* pointLight = static_cast<PointLightComponent*>(lightComponent);
		const float range = pointLight->GetRange();
		const XMFLOAT3 position = pointLight->GetPosition();

		XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), 1.0f, 1.0f, range);

		XMVECTOR lightPosition = XMLoadFloat3(&position);
		XMVECTOR upDirection1 = { 0.0f, 1.0f, 0.0f };
		XMVECTOR upDirection2 = { 0.0f, 0.0f, -1.0f };
		XMVECTOR upDirection3 = { 0.0f, 0.0f, 1.0f };

		XMVECTOR left = { position.x + range, position.y + 0.0f, position.z + 0.0f };
		XMVECTOR right = { position.x + -range, position.y + 0.0f, position.z + 0.0f };
		XMVECTOR up = { position.x + 0.0f, position.y + range, position.z + 0.0f };
		XMVECTOR down = { position.x + 0.0f, position.y + -range, position.z + 0.0f };
		XMVECTOR front = { position.x + 0.0f, position.y + 0.0f, position.z + range };
		XMVECTOR back = { position.x + 0.0f, position.y + 0.0f, position.z + -range };

		XMMATRIX leftView = XMMatrixLookAtLH(lightPosition, left, upDirection1);
		XMMATRIX rightView = XMMatrixLookAtLH(lightPosition, right, upDirection1);
		XMMATRIX upView = XMMatrixLookAtLH(lightPosition, up, upDirection2);
		XMMATRIX downView = XMMatrixLookAtLH(lightPosition, down, upDirection3);
		XMMATRIX frontView = XMMatrixLookAtLH(lightPosition, front, upDirection1);
		XMMATRIX backView = XMMatrixLookAtLH(lightPosition, back, upDirection1);

		XMFLOAT4X4 viewProjectionMatrices[6] = {};
		XMStoreFloat4x4(&viewProjectionMatrices[0], XMMatrixTranspose(leftView * projection));
		XMStoreFloat4x4(&viewProjectionMatrices[1], XMMatrixTranspose(rightView * projection));
		XMStoreFloat4x4(&viewProjectionMatrices[2], XMMatrixTranspose(upView * projection));
		XMStoreFloat4x4(&viewProjectionMatrices[3], XMMatrixTranspose(downView * projection));
		XMStoreFloat4x4(&viewProjectionMatrices[4], XMMatrixTranspose(frontView * projection));
		XMStoreFloat4x4(&viewProjectionMatrices[5], XMMatrixTranspose(backView * projection));

		shadowViewProjectionMatrices.assign(&viewProjectionMatrices[0], &viewProjectionMatrices[6]);
	}

	if (isDirectional) {
		float sceneRadius = 20.0f;
		const XMFLOAT3 sceneCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//generate view matrix
		const DirectionalLightComponent* light = static_cast<DirectionalLightComponent*>(lightComponent);

		const XMFLOAT3 lightDirectionStorage = light->GetDirection();
		const XMVECTOR lightDirection = XMLoadFloat3(&lightDirectionStorage);
		const XMVECTOR lightPosition = -2.0f * sceneRadius * lightDirection;
		const XMVECTOR targetPostion = XMLoadFloat3(&sceneCenter);
		const XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		const XMMATRIX lightView = XMMatrixLookAtLH(lightPosition, targetPostion, up);

		//generate projection matrix
		XMFLOAT3 sphereCenterLS;
		XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPostion, lightView));
		const float left = sphereCenterLS.x - sceneRadius;
		const float right = sphereCenterLS.x + sceneRadius;
		const float bottom = sphereCenterLS.y - sceneRadius;
		const float top = sphereCenterLS.y + sceneRadius;
		const float nearPlane = sphereCenterLS.z - sceneRadius;
		const float farPlane = sphereCenterLS.z + sceneRadius;

		XMMATRIX lightProj = XMMatrixOrthographicOffCenterLH(left, right, bottom, top, nearPlane, farPlane);
		XMStoreFloat4x4(&shadowViewMatrix, lightView);
		XMStoreFloat4x4(&shadowViewProjectionMatrix, lightProj);
	}
}

bool ShadowInfo::GetIsOmniDirectionalShadow() const {
	return isOmniDirectionalShadow;
}

bool ShadowInfo::GetIsProjectedShadow() const {
	return isProjectedShadow;
}

XMFLOAT4X4 ShadowInfo::GetShadowViewProjectionMatrix() const {
	return shadowViewProjectionMatrix;
}

XMFLOAT4X4 ShadowInfo::GetShadowViewMatrix() const {
	return shadowViewMatrix;
}

std::vector<XMFLOAT4X4> ShadowInfo::GetShadowViewProjectionMatrices() const {
	return shadowViewProjectionMatrices;
}
