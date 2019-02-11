#include "ResourceManager.h"
#include "Renderer.h"

void ResourceManager::AddTexture(std::string name, TextureRI * texture) {
	textureCache.insert(std::pair<std::string, TextureRI *>(name, texture));
}

TextureRI* ResourceManager::GetTexture(std::string name) const {
	return textureCache.at(name);
}

void ResourceManager::DestroyTexture(std::string name) {
	TextureRI* texture = textureCache.at(name);
	textureCache.erase(name);

	delete texture;
	texture = nullptr;
}

void ResourceManager::DestroyAllTextures() {
	std::map<std::string, TextureRI* >::iterator it;
	it = textureCache.begin();

	while (it != textureCache.end()) {
		TextureRI* texture = it->second;
		textureCache.erase(it);

		delete texture;
		texture = nullptr;
		it = textureCache.begin();
	}
}

void ResourceManager::CreateTextureFromFile(char* fileName, bool generateMipMaps) {
	Image image = {};
	image.LoadImageFromFile(fileName, generateMipMaps);

	Texture2DRI* texture = renderingInterface->CreateTexture2d(image.GetWidth(), image.GetHeight(), image.GetArraySize(), image.IsCube(), false, image.GetMipMapCount(), image.GetFormat(), TextureBindAsShaderResource, 1, image.GetImageData());
	AddTexture(fileName, texture);
}

void ResourceManager::GetShaderByteCode(const char * fileName, ShaderStages shaderStage, const ShaderMacro* macros, unsigned int macroCount, unsigned int * byteCodeSize, char ** byteCode) const {
	File file = {};

	file.Open(fileName);
	std::string line = "";

	while (!file.IsEof()) {
		line += file.ReadLine() + "\n";
	}

	file.Close();

	renderingInterface->CompileShader(shaderStage, line.size(), fileName, line.data(), macros, macroCount, byteCodeSize, byteCode);
}
