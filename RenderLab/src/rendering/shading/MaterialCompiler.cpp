#include "MaterialCompiler.h"

MaterialCompiler::MaterialCompiler() {}

MaterialCompiler::~MaterialCompiler() {}

bool MaterialCompiler::CompileMaterial(Material * material) {
	generatedShader = "";
	ReadShaderCode("shaders/BasicPixelShader.hlsl", &generatedShader);
	
	std::vector<const TextureSamplerNode*> allTextureNodes;
	material->GetAllNodesOfType<TextureSamplerNode>(allTextureNodes);
	CreateTextureUniforms(material, allTextureNodes);
	WriteAlbedo(material->GetAlbedo(), &generatedShader);

	char* byteCode = nullptr;
	unsigned int byteCodeSize = 0;

	ShaderMacro macro0 = {};
	macro0.name = "USE_NORMAL_MAP";
	macro0.definition = "1";

	ShaderMacro macro1 = {};
	macro1.name = "TEXTURE_2D_00";
	macro1.definition = "1";

	ShaderMacro macros[2];
	macros[0] = macro0;
	macros[1] = macro1;

	CompileShader("shaders/BasicPixelShader.hlsl", PIXEL_SHADER, macros, 2, &byteCodeSize, &byteCode);
	char* pixelCode = byteCode;
	PixelShader* pixelShader = renderingInterface->CreatePixelShader(pixelCode, byteCodeSize);

	material->SetShader(pixelShader);
	return true;
}

void MaterialCompiler::ReadShaderCode(const char * fileName, std::string* outReadShaderCod) const {
	File file = {};

	file.Open(fileName);

	while (!file.IsEof()) {
		*outReadShaderCod += file.ReadLine() + "\n";
	}

	file.Close();
}

void MaterialCompiler::CreateTextureUniforms(Material* material, std::vector<const TextureSamplerNode*>& sampleNodes) const {
	std::vector<TextureRI*> textures;

	const unsigned int reservedTextureUnits = 2;

	bool isSame = false;
	for (const TextureSamplerNode* currentNode : sampleNodes) {
		isSame = false;

		for (std::vector<TextureRI*>::size_type i = 0; i != textures.size(); i++) {
			if (currentNode->GetTexture() == textures[i]) {
				isSame = true;
				break;
			}
		}

		if (!isSame) {
			textures.push_back(currentNode->GetTexture());
		}
	}

	for (std::vector<const TextureRI*>::size_type i = 0; i != textures.size(); i++) {
		MaterialTextureUniform* textureUniform = new MaterialTextureUniform(textures[i], i);
		material->AddTextureUniform(textureUniform);
	}
}

void MaterialCompiler::WriteAlbedo(MaterialNode * node, std::string* code) {
	std::string albedoFunction = "GetAlbedo(PixelIn vIn) {";
	size_t pos = code->find(albedoFunction);

	const std::string albedo = node->GetExpression();

	code->insert(pos + albedoFunction.length(), albedo);
}

void MaterialCompiler::CompileShader(const char * fileName, ShaderStages shaderStage, const ShaderMacro * macros, unsigned int macroCount, unsigned int * byteCodeSize, char ** byteCode) {
	renderingInterface->CompileShader(shaderStage, generatedShader.size(), fileName, generatedShader.data(), macros, macroCount, byteCodeSize, byteCode);
}
