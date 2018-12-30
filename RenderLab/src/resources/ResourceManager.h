#pragma once
#include "RenderingInterfaceResources.h"
#include "FileSystem.h"
#include "Image.h"
#include <map>
class ResourceManager {
public:
public:
	static ResourceManager& GetInstance() {
		static ResourceManager instance;

		return instance;
	}

private:
	ResourceManager() {}
	ResourceManager(ResourceManager const&);
	void operator=(ResourceManager const&);

public:
	void CreateTextureFromFile(char* fileName, bool generateMipMaps);
	void GetShaderByteCode(const char* fileName, ShaderStages shaderStage, const ShaderMacro* macros, unsigned int macroCount, unsigned int* byteCodeSize, char** byteCode) const;

	void AddTexture(std::string name, TextureRI* texture);
	TextureRI* GetTexture(std::string name) const;
	void DestroyTexture(std::string name);
	void DestroyAllTextures();

private:
	std::map<std::string, TextureRI*> textureCache;
};
