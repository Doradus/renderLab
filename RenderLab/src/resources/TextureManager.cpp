#include "TextureManager.h"

void TextureManager::AddTexture(std::string name, TextureRI * texture) {
	textures.insert(std::pair<std::string, TextureRI *>(name, texture));
}

TextureRI* TextureManager::GetTexture(std::string name) const {
	return textures.at(name);
}

void TextureManager::DestroyTexture(std::string name) {
	TextureRI* texture = textures.at(name);
	textures.erase(name);
	
	delete texture;
	texture = nullptr;
}

void TextureManager::DestroyAllTextures() {
	std::map<std::string, TextureRI* >::iterator it;
	it = textures.begin();

	while (it != textures.end()) {
		TextureRI* texture = it->second;
		textures.erase(it);

		delete texture;
		texture = nullptr;
		it = textures.begin();
	}
}

