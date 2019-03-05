#pragma once
#include <string>
#include "MathUtils.h"

class Material;
class MaterialNode {
public:
	MaterialNode(class Material* material);
	virtual ~MaterialNode();

	virtual void			GetValue(DirectX::XMFLOAT4* outValue) const = 0;
	virtual	int				Compile(class MaterialCompiler* compiler) = 0;

protected:
	Material*	owner;
	int			codeIndex;

private:
	std::string name;
};