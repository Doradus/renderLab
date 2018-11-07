#pragma once
#include <Windows.h>
class Shader {
public:
	Shader(char* inSrcCode);
	virtual ~Shader();

private:
	char* srcCode;
};
