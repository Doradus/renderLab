#pragma once
#include "ShaderCode.h"
#include "Material.h"
#include "FileSystem.h"
#include "MaterialUniform.h"

class MaterialCompiler {
public:
	MaterialCompiler();
	~MaterialCompiler();

	bool CompileMaterial(Material* material);

	//nodes
	int		Scalar(float value);
	int		Vector3(float r, float g, float b);
	int		TextureSampler(int textureIndex);
	int		Blank();

private:
	void		ReadShaderCode(const char * fileName, std::string* outReadShaderCode) const;
	void		CreateTextureUniforms(Material* material) const;
	void		CompileShader(const char * fileName, ShaderStages shaderStage, const ShaderMacro* macros, unsigned int macroCount, unsigned int * byteCodeSize, char ** byteCode);
	int			AddCode(const std::string& code);
	bool		WriteShaderCode(std::string* shader, int codeIndex, std::string type);
	std::vector<ShaderCode*>	generatedCode;
	std::string					generatedShader;
};
