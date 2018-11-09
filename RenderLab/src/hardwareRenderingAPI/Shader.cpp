#include "Shader.h"

Shader::Shader(char * inSrcCode) :
srcCode (inSrcCode) {}

Shader::~Shader() {
	if (srcCode != nullptr) {
		free(srcCode);
		srcCode = nullptr;
	}
}
