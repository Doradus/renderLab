#include "MaterialUniform.h"


MaterialTextureUniform::MaterialTextureUniform(TextureRI* inTexture, unsigned int textureIndex) : 
	texture (inTexture),
	inputSlot (textureIndex)
{}

MaterialTextureUniform::~MaterialTextureUniform() {}

void MaterialTextureUniform::AddTexture(TextureRI * inTexture, unsigned int textureIndex) {
	texture = inTexture;
	inputSlot = textureIndex;
}

void MaterialTextureUniform::BindResource() const {
	renderingInterface->SetShaderResources(texture, 2);
}


