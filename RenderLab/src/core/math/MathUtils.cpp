#include "MathUtils.h"

const float MathUtils::PI = 3.1415926535f;

static float DegreeToRadians(float degree) {
	return degree * MathUtils::PI / 180;
}
