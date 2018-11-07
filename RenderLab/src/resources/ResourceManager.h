#pragma once
#include "RenderingInterfaceResources.h"
#include "FileSystem.h"


class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	void GetShaderByteCode(const char* fileName, ShaderStages shaderStage, unsigned int* byteCodeSize, char** byteCode) const;
};
