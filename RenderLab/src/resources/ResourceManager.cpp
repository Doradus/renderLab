#include "ResourceManager.h"
#include "Renderer.h"

ResourceManager::ResourceManager()
{}

ResourceManager::~ResourceManager() {
}

void ResourceManager::GetShaderByteCode(const char * fileName, ShaderStages shaderStage, unsigned int * byteCodeSize, char ** byteCode) const {
	File file = {};

	file.Open(fileName);
	std::string line = "";

	while (!file.IsEof()) {
		line += file.ReadLine() + "\n";
	}

	file.Close();

	renderingInterface->CompileShader(shaderStage, line.size(), fileName, line.data(), nullptr, 0, byteCodeSize, byteCode);
}
