#pragma once
#include "FileSystem.h"

#define DDS_MAGIC 0x20534444
#define MAKE_CHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))
#define DDSCAPS2_CUBEMAP 0x200


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
		DXT5,

		DEPTH_STENCIL,
		SHADOW_DEPTH
	};

	bool IsCompressedFormat(Format format);
	unsigned int GetBytesPerPixel(Format format);
	unsigned int GetBytesPerBlock(Format format);
	unsigned int GetSize(Format format);
	unsigned int GetBlockWidth(Format format);
	unsigned int GetWidth(Format format, unsigned int width);
}

class Image {
public:
	Image();
	~Image();

	bool					LoadImageFromFile(const char* fileName, bool useMipMaps);

	unsigned int			GetWidth() const;
	unsigned int			GetHeight() const;
	unsigned int			GetDepth() const;
	unsigned int			GetMipMapCount() const;
	unsigned int			GetArraySize() const;
	ImageFormats::Format	GetFormat() const;
	unsigned char*			GetImageData() const;
	bool					IsCube() const;

private:
	bool CreateDDSFromMemory(const char* data, size_t memsize, bool useMipMaps);
	unsigned int GetImageSize() const;

private:
	unsigned char*			imageData;

	unsigned int			width;
	unsigned int			height;
	unsigned int			depths;
	unsigned int			mipMapCount;
	unsigned int			arraySize;
	
	ImageFormats::Format	format;
};