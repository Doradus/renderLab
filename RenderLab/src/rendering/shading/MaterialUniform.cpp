#include "MaterialUniform.h"


MaterialUniform::MaterialUniform(Material* inMaterial) :
	owner (inMaterial) {}

MaterialUniform::~MaterialUniform() {}

MaterialTextureUniform::MaterialTextureUniform(TextureRI* inTexture, unsigned int textureIndex, Material* inMaterial) :
	MaterialUniform(inMaterial),
	texture (inTexture),
	inputSlot (textureIndex)
{}

MaterialTextureUniform::~MaterialTextureUniform() {}

void MaterialTextureUniform::AddTexture(TextureRI * inTexture, unsigned int textureIndex) {
	texture = inTexture;
	inputSlot = textureIndex;
}

void MaterialTextureUniform::BindResource() const {
	//hardcoded for now should be removed
	unsigned int textureOffset = 3;

	int textureIndex = owner->GetTextureIndex(texture);

	renderingInterface->SetShaderResources(texture, textureIndex + textureOffset);
}


