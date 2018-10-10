#pragma once
#include "FileSystem.h"

#define DDS_MAGIC 0x20534444
#define MAKE_CHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

namespace ImageFormats {
	enum Format {
		NONE,
		R8,
		RG8,
		RGB8,
		RGBA8,

		R16,
		RG16,
		RGB16,
		RGBA16,

		DXT1,
		DXT2,
		DXT3,
		DXT4,
		DXT5
	};
}

class Image {
public:
	Image();
	~Image();

	bool LoadImageFromFile(const char* fileName, bool useMipMaps);

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetDepth() const;
	unsigned int GetMipMapCount() const;
	ImageFormats::Format GetFormat() const;
	unsigned char* GetImageData() const;

private:
	bool CreateDDSFromMemory(const char* data, size_t memsize, bool useMipMaps);

private:
	unsigned char* imageData;

	unsigned int width;
	unsigned int height;
	unsigned int depths;
	unsigned int mipMapCount;
	ImageFormats::Format format;
};