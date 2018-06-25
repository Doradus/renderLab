#pragma once
#include "SceneComponent.h"
class LightComponent : public SceneComponent  {
public:
	LightComponent();
	virtual ~LightComponent();

	void		SetLightColor(const XMFLOAT3& color);
	void		SetLightColor(float r, float g, float b);
	void		SetBrightness(float value);
	void		SetAttenuation(const XMFLOAT3& value);
	void		SetAttenuation(float value1, float value2, float value3);
	void		SetCastsShadows(bool value);
	void		SetCastDynamicShadows(bool value);
	void		SetIsEnabled(bool value);

	XMFLOAT3	GetLightColor() const;
	float		GetBrightness() const;
	XMFLOAT3	GetAttenuation() const;
	bool		GetsCastsShadows() const;
	bool		GetsCastsDynamicShadows() const;
	bool		GetIsEnabled() const;

protected:
	XMFLOAT3 lightColor;
	XMFLOAT3 attenuation;
	float brightness;
	bool castsShadows;
	bool castDynamicShadows;
	bool isEnabled;
};