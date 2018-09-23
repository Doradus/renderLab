#pragma once
#include <vector>
#include "LightComponent.h"

class PointLightComponent : public LightComponent {
public:
	PointLightComponent();
	~PointLightComponent();

	void SetRange(const float& inRange);
	void UpdateTransform() override;
	void UpdateProjectionMatrix();
	void UpdateViewMatrix();

	float GetRange() const;

	std::vector<XMFLOAT4X4> GetViewMatrix() const;
	XMFLOAT4X4 GetProjectionMatrix() const;

protected:
	XMFLOAT4X4 transformCache;

private:
	float range;
	std::vector<XMFLOAT4X4> viewMatrix;
	XMFLOAT4X4 projectionMatrix;
};