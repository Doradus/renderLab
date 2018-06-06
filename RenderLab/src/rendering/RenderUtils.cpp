#include "RenderUtils.h"

TextureFormatInfo GTextureFormatInfo[32] = {
	{"unknown", 0, 0, UNKNOWN},
	{"R8G8B8A8_UINT", 0, 0, R8G8B8A8_UINT},
	{"R8G8B8A8_SNORM", 0, 0, R8G8B8A8_SNORM},
	{"depthStencil", 0, 0, DEPTH_STENCIL},
	{"shadowDepth", 0, 0, SHADOW_DEPTH},
};