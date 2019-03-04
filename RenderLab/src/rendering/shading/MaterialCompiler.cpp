#include "MaterialCompiler.h"

MaterialCompiler::MaterialCompiler() {
	AddCode("");
}

MaterialCompiler::~MaterialCompiler() {}

bool MaterialCompiler::CompileMaterial(Material * material) {
	generatedShader = "";
	ReadShaderCode("shaders/PixelShader.hlsl", &generatedShader);
	
	CreateTextureUniforms(material);

	int albedo = material->CompileAlbedo(this);
	int normal = material->CompileNormal(this);
	int roughness = material->CompileRoughness(this);

	WriteShaderCode(&generatedShader, albedo, "%Albedo%");
	WriteShaderCode(&generatedShader, normal, "%Normal%");
	WriteShaderCode(&generatedShader, roughness, "%Roughness%");

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


int MaterialCompiler::Scalar(float value) {
	std::string code = "return float4(" + std::to_string(value) + ", " + std::to_string(value) + ", " + std::to_string(value) + ", " + std::to_string(value) + ");";
	return AddCode(code);
}

int MaterialCompiler::Vector3(float r, float g, float b) {
	std::string code = "return float4(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", 0.0f);";
	return AddCode(code);
}

int MaterialCompiler::TextureSampler(int textureIndex) {
	std::string code = "return texture0" + std::to_string(textureIndex) + ".Sample(textureSampler, vIn.uv);";
	return AddCode(code);
}

int MaterialCompiler::Blank() {
	return 0;
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

void MaterialCompiler::CompileShader(const char * fileName, ShaderStages shaderStage, const ShaderMacro * macros, unsigned int macroCount, unsigned int * byteCodeSize, char ** byteCode) {
	renderingInterface->CompileShader(shaderStage, generatedShader.size(), fileName, generatedShader.data(), macros, macroCount, byteCodeSize, byteCode);
}

int MaterialCompiler::AddCode(const std::string& code) {
	ShaderCode* codeShunk = new ShaderCode(code);

	generatedCode.push_back(codeShunk);

	return generatedCode.size() - 1;
}

bool MaterialCompiler::WriteShaderCode(std::string * shader, int codeIndex, std::string type) {
	std::string function = type;
	size_t pos = shader->find(function);

	if (pos == std::string::npos) {
		return false;
	}

	shader->replace(pos, function.length(), generatedCode[codeIndex]->code);

	return true;
}
