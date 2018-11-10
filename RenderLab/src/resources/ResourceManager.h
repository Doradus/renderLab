#pragma once
#include "RenderingInterfaceResources.h"
#include "FileSystem.h"


class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	void GetShaderByteCode(const char* fileName, ShaderStages shaderStage, const ShaderMacro* macros, unsigned int macroCount, unsigned int* byteCodeSize, char** byteCode) const;
};
