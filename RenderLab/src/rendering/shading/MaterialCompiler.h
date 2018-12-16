#pragma once
#include "Material.h"
#include "FileSystem.h"

class MaterialCompiler {
public:
	MaterialCompiler();
	~MaterialCompiler();

	bool CompileMaterial(Material* material);

private:
	void		ReadShaderCode(const char * fileName, std::string* outReadShaderCode) const;
	void		CreateTextureUniforms(Material* material, std::vector<const TextureSamplerNode*>& sampleNodes) const;
	void		WriteAlbedo(MaterialNode* node, std::string* code);
	void		CompileShader(const char * fileName, ShaderStages shaderStage, const ShaderMacro* macros, unsigned int macroCount, unsigned int * byteCodeSize, char ** byteCode);

	std::string generatedShader;
};
