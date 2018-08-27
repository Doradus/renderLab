#include "ShadowInfo.h"
#include "PointLightComponent.h"

ShadowInfo::ShadowInfo() : 
	isCascadingShadow (false),
	isOmniDirectionalShadow(false),
	shadowId(0)
{
	shadowViewProjectionMatrices = {};
}

ShadowInfo::~ShadowInfo() {
}

void ShadowInfo::CreateShadowInfo(LightComponent * lightComponent) {
	switch (lightComponent->GetLightType()) {
	case SPOT_LIGHT : 
		isSpot = true;
		break;
	case POINT_LIGHT :
		isOmniDirectionalShadow = true;
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
		XMVECTOR up = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));

		XMMATRIX view = XMMatrixLookToLH(lightPosition, lightDirection, up);

		XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(coneAngle + penumbraAngle), 1.0, 1.0f, range);

		XMStoreFloat4x4(&shadowViewProjectionMatrix, XMMatrixTranspose(view * projection));
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
}

bool ShadowInfo::GetIsOmniDirectionalShadow() const {
	return isOmniDirectionalShadow;
}

std::vector<XMFLOAT4X4> ShadowInfo::GetShadowViewProjectionMatrices() const {
	return shadowViewProjectionMatrices;
}
