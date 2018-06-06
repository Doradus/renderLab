#pragma once

struct TextureFormatInfo {
	const char* name;
	bool isSupported;
	unsigned int platformFormat;
	unsigned char labTextureFormat;
};

enum LabTextureFormat {
	UNKNOWN,
	R8G8B8A8_UINT,
	R8G8B8A8_SNORM,
	DEPTH_STENCIL,
	SHADOW_DEPTH
};

enum TextureBindFlags {
	TextureBindAsShaderResource = 0,
	TextureBindAsRenderTarget = 1 << 0,
	TextureBindAsDepthStencil = 1 << 1
};

extern TextureFormatInfo GTextureFormatInfo[32];