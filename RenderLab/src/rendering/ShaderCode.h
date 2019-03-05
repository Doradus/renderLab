#pragma once
#include <string>

class ShaderCode {
public:
	ShaderCode(const std::string& inCode);
	~ShaderCode();

	std::string code;
};
