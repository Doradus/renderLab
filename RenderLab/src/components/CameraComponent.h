#pragma once
#include "SceneComponent.h"

class CameraComponent : public SceneComponent {
public:
	CameraComponent();
	CameraComponent(float aspectRatio, float nearPlane, float farPlane, float fieldOfView);
	~CameraComponent();

	void		UpdateProjection();
	void		UpdateView();

	void		SetAspectratio(float ratio);
	void		SetNearPlane(float value);
	void		SetFarPlane(float value);
	void		SetFieldOfView(float value);

	void		SetCameraTarget(XMFLOAT3 target);
	void		SetCameraTarget(float x, float y, float z);

	XMFLOAT4X4	GetProjection() const;
	XMFLOAT4X4	GetCameraView() const;
	XMFLOAT3	GetCameraTarget() const;

	XMFLOAT4X4 projectionMatrix;
	XMFLOAT4X4 cameraView;
	XMFLOAT3 upDirection;
	XMFLOAT3 cameraTarget;

private:
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float fieldOfView;
};