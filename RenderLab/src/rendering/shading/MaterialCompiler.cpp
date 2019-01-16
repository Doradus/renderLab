#include "MaterialCompiler.h"

MaterialCompiler::MaterialCompiler() {}

MaterialCompiler::~MaterialCompiler() {}

bool MaterialCompiler::CompileMaterial(Material * material) {
	generatedShader = "";
	ReadShaderCode("shaders/BasicPixelShader.hlsl", &generatedShader);
	
	CreateTextureUniforms(material);
	WriteAlbedo(material->GetAlbedo(), &generatedShader);
	WriteNormal(material, &generatedShader);
	WriteRoughness(material, &generatedShader);

	char* byteCode = nullptr;
	unsigned int byteCodeSize = 0;

	std::vector<ShaderMacro> macros;

	ShaderMacro macro0 = {};
	macro0.name = "USE_NORMAL_MAP";

	if (material->UseNormals()) {
		macro0.definition = "1";
	} else {
		macro0.definition = "0";
	}

	macros.push_back(macro0);

	//texture resources

	unsigned int texuresUsed = material->GetNumberOfTexturesUsed();

	for (unsigned int i = 0; i < texuresUsed; i++) {
		ShaderMacro macro = {};
		macro.name = "TEXTURE_2D_0" + std::to_string(i);
		macro.definition = "1";

		macros.push_back(macro);
	}

	CompileShader("shaders/BasicPixelShader.hlsl", PIXEL_SHADER, macros.data(), macros.size(), &byteCodeSize, &byteCode);
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

void MaterialCompiler::CreateTextureUniforms(Material* material) const {
	for (std::vector<const TextureRI*>::size_type i = 0; i != material->GetTextureResources().size(); i++) {
		MaterialTextureUniform* textureUniform = new MaterialTextureUniform(material->GetTextureResources()[i], i, material);
		material->AddTextureUniform(textureUniform);
	}
}

void MaterialCompiler::WriteAlbedo(MaterialNode * node, std::string* code) {
	std::string albedoFunction = "%Albedo%";
	size_t pos = code->find(albedoFunction);

	const std::string albedo = node->GetExpression();

	code->replace(pos, albedoFunction.length(), albedo);
}

void MaterialCompiler::WriteNormal(Material* material, std::string * code) {
	std::string normalFunction = "%Normal%";
	size_t pos = code->find(normalFunction);

	if (material->UseNormals()) {
		const std::string normal = material->GetNormal()->GetExpression();
		code->replace(pos, normalFunction.length(), normal);
	} else {
		code->replace(pos, normalFunction.length(), "");
	}
}

void MaterialCompiler::WriteRoughness(Material * material, std::string * code) {
	std::string roughnessFunction = "%Roughness%";
	size_t pos = code->find(roughnessFunction);

	if (material->UseRoughness()) {
		const std::string normal = material->GetRougness()->GetExpression();
		code->replace(pos, roughnessFunction.length(), normal);
	}
	else {
		code->replace(pos, roughnessFunction.length(), "");
	}
}

void MaterialCompiler::CompileShader(const char * fileName, ShaderStages shaderStage, const ShaderMacro * macros, unsigned int macroCount, unsigned int * byteCodeSize, char ** byteCode) {
	renderingInterface->CompileShader(shaderStage, generatedShader.size(), fileName, generatedShader.data(), macros, macroCount, byteCodeSize, byteCode);
}
