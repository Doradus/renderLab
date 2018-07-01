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
	None = 0,
	TextureBindAsShaderResource = 1 << 0,
	TextureBindAsRenderTarget = 1 << 1,
	TextureBindAsDepthStencil = 1 << 2
};

extern TextureFormatInfo GTextureFormatInfo[32];