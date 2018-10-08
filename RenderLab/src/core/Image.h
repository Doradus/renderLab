#pragma once
#include "FileSystem.h"

#define DDS_MAGIC 0x20534444
#define MAKE_CHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

class Image {
public:
	Image();
	~Image();

	bool LoadImageFromFile(const char* fileName, bool useMipMaps);

private:
	bool CreateDDSFromMemory(const char* data, size_t memsize, bool useMipMaps);

private:
	unsigned char* imageData;

	unsigned int width;
	unsigned int height;
	unsigned int depths;
	unsigned int mipMapCount;
};