#include "PointLightComponent.h"

PointLightComponent::PointLightComponent() {

}

PointLightComponent::~PointLightComponent() {

}

void PointLightComponent::SetRange(const float & inRange) {
	range = inRange;
}

float PointLightComponent::GetRange() const {
	return range;
}
