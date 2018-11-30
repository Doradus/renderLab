#pragma once
#include "RenderingInterfaceResources.h"
#include <map>
class TextureManager {
public:
	static TextureManager& GetInstance() {
		static TextureManager instance;

		return instance;
	}

private:
	TextureManager() {}
	TextureManager(TextureManager const&);
	void operator=(TextureManager const&);

	std::map<std::string, TextureRI*> textures;

public:
//	TextureManager(TextureManager const&) = delete;
//	void operator=(TextureManager const&) = delete;

	void AddTexture(std::string name, TextureRI* texture);
	TextureRI* GetTexture(std::string name) const;
	void DestroyTexture(std::string name);
	void DestroyAllTextures();
};
