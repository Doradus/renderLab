#include "RenderUtils.h"

TextureFormatInfo GTextureFormatInfo[32] = {
	{ImageFormats::NONE, 0, 0},
	{ImageFormats::RGB8, 0, 0},
	{ImageFormats::RGBA8, 0, 0},
	{ImageFormats::DXT1, 0, 0 },
	{ImageFormats::DXT3, 0, 0 },
	{ImageFormats::DXT5, 0, 0 },
	{ImageFormats::DEPTH_STENCIL, 0, 0},
	{ImageFormats::SHADOW_DEPTH, 0, 0},
};