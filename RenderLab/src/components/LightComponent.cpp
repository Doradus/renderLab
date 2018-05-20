#include "LightComponent.h"

LightComponent::LightComponent() {
}

LightComponent::~LightComponent() {
}

void LightComponent::UpdateView() {
}

void LightComponent::UpdateProjection() {
}

void LightComponent::SetLightColor(XMFLOAT3 color) {
	lightColor = color;
}

void LightComponent::SetLightColor(float r, float g, float b) {
	lightColor.x = r;
	lightColor.y = g;
	lightColor.z = b;
}

void LightComponent::SetBrightness(float value) {
	brightness = value;
}

void LightComponent::SetAttenuation(XMFLOAT3 value) {
	attenuation = value;
}

void LightComponent::SetAttenuation(float value1, float value2, float value3) {
	attenuation.x = value1;
	attenuation.y = value2;
	attenuation.z = value3;
}

void LightComponent::SetCastsShadows(bool value) {
	castsShadows = value;
}

void LightComponent::SetCastDynamicShadows(bool value) {
	castDynamicShadows = value;
}

void LightComponent::SetIsEnabled(bool value) {
	isEnabled = value;
}

XMFLOAT3 LightComponent::GetLightColor() const {
	return lightColor;
}

float LightComponent::GetBrightness() const {
	return brightness;
}

XMFLOAT3 LightComponent::GetAttenuation() const {
	return attenuation;
}

bool LightComponent::GetsCastsShadows() const {
	return castsShadows;
}

bool LightComponent::GetsCastsDynamicShadows() const {
	return castDynamicShadows;
}

bool LightComponent::GetIsEnabled() const {
	return isEnabled;
}
