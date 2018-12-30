#pragma once
#include "MathUtils.h"
#include "RenderingInterfaceResources.h"

template <class T>
class MaterialValue {
public:
	MaterialValue::MaterialValue(T inValue) : 
		value (inValue)
	{
	}

	MaterialValue::~MaterialValue()
	{
	}

	void SetValue(T inValue) {
		value = inValue;
	}

	T GetValue() const {
		return value;
	}

private:
	T value;
};


class ColorValue : public MaterialValue<DirectX::XMFLOAT4> {};

class Vector3Value : public MaterialValue<DirectX::XMFLOAT3> {};

class Vector2Value : public MaterialValue<DirectX::XMFLOAT2> {};

class ScalarValue : public MaterialValue<unsigned int> {};

class TextureValue : public MaterialValue<TextureRI> {};