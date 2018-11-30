#include "MaterialCompiler.h"

MaterialCompiler::MaterialCompiler() {}

MaterialCompiler::~MaterialCompiler() {}

bool MaterialCompiler::CompileMaterial(Material * material) {
	generatedShader = "";
	ReadShaderCode("shaders/BasicPixelShader.hlsl", &generatedShader);
	
	WriteAlbedo(material->GetAlbedo(), &generatedShader);

	char* byteCode = nullptr;
	unsigned int byteCodeSize = 0;

	ShaderMacro macro = {};
	macro.name = "USE_NORMAL_MAP";
	macro.definition = "1";

	ShaderMacro macros[1];
	macros[0] = macro;

	CompileShader("shaders/BasicPixelShader.hlsl", PIXEL_SHADER, macros, 1, &byteCodeSize, &byteCode);
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

void MaterialCompiler::WriteAlbedo(MaterialNode * node, std::string* code) {
	size_t pos = code->find("GetAlbedo");

	const std::string albedo = "return diffuseTexture.Sample(textureSampler, vIn.uv).rgb;";

	code->insert(pos + 25, albedo);
}

void MaterialCompiler::CompileShader(const char * fileName, ShaderStages shaderStage, const ShaderMacro * macros, unsigned int macroCount, unsigned int * byteCodeSize, char ** byteCode) {
	renderingInterface->CompileShader(shaderStage, generatedShader.size(), fileName, generatedShader.data(), macros, macroCount, byteCodeSize, byteCode);
}
