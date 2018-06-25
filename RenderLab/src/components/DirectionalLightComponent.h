#pragma once
#include "LightComponent.h"
#include "Texture2D.h"
#include "RenderingInterface.h"

class DirectionalLightComponent : public LightComponent  {
public:
	DirectionalLightComponent();
	~DirectionalLightComponent();

	void			UpdateView();
	void			UpdateProjection();

	void			SetDirection(XMFLOAT3 inDirection);
	void			SetDirection(float x, float y, float z);
	void			InitShadowMap();
	XMFLOAT3		GetDirection() const;
	Texture2DRI*	GetShadowMap() const;

private:
	XMFLOAT4X4		viewMatrix;
	XMFLOAT4X4		projectionMatrix;
	XMFLOAT3		direction;
	Texture2DRI*	shadowMap;
};