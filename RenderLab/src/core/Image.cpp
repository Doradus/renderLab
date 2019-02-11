#include "Image.h"
#include <Windows.h>

struct DDS_PIXEL_FORMAT {
	unsigned int size;
	unsigned int flags;
	unsigned int fourCC;
	unsigned int rgbBitCount;
	unsigned int rBitMask;
	unsigned int gBitMask;
	unsigned int bBitMask;
	unsigned int aBitMask;
};

struct DDS_HEADER {
	unsigned int mDWMagic;
	unsigned int size;
	unsigned int flags;
	unsigned int height;
	unsigned int width;
	unsigned int pitchOrLinearSize;
	unsigned int depth;
	unsigned int mipMapCount;
	unsigned int reserved[11];
	DDS_PIXEL_FORMAT pixelFormat;
	unsigned int caps1;
	unsigned int caps2;
	unsigned int caps3;
	unsigned int caps4;
	unsigned int reserved2;
};

struct	DDS_HEADER_DX10 {
	unsigned int dxgiFormat;
	unsigned int resourceDimension;
	unsigned int miscFlags;
	unsigned int arraySize;
	unsigned int reserved;
};

Image::Image() :
imageData (nullptr)  {
}

Image::~Image() {
	if (imageData) {
		free(imageData);
		imageData = nullptr;
	}
}

bool Image::CreateDDSFromMemory(const char * data, size_t memsize, bool useMipMaps) {
	DDS_HEADER header = {};

	MemoryBuffer buffer(data);

	buffer.Read(&header, sizeof(header));

	if (header.mDWMagic != DDS_MAGIC) {
		return false;
	}

	width = header.width;
	height = header.height;
	depths = header.depth;
	mipMapCount = (useMipMaps == false || header.mipMapCount == 0) ? 1 : header.mipMapCount;

	if (header.pixelFormat.fourCC == MAKE_CHAR4('D', 'X', '1', '0')) {
		MessageBox(0, L"DX10 Header", 0, 0);
	}

	bool isCubeMap = false;
	arraySize = 1;
	if (header.caps2 & DDSCAPS2_CUBEMAP) {
		isCubeMap = true;
		arraySize = 6;
	}

	switch (header.pixelFormat.fourCC) {
	case MAKE_CHAR4('D', 'X', 'T', '1') :
		format = ImageFormats::DXT1;
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '2'):
		format = ImageFormats::DXT2;
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '3'):
		format = ImageFormats::DXT3;
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '4'):
		format = ImageFormats::DXT4;
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '5'):
		format = ImageFormats::DXT5;
		break;
	default:
		switch (header.pixelFormat.rgbBitCount) {
		case 24 : 
			format = ImageFormats::RGB8;
			break;
		case 32 :
			format = ImageFormats::RGBA8;
			break;
		default:
			return false;
		}
	}

	unsigned int imageDataSize = GetImageSize() * arraySize;

	if (imageDataSize == 0) {
		return false;
	}

	imageData = (unsigned char*) malloc(imageDataSize);
	buffer.Read(imageData, imageDataSize);

	return true;
}

unsigned int Image::GetImageSize() const {
	unsigned int currentMipMapWidth = width;
	unsigned int currentMipMapHeight = height;
	unsigned int accumalatedSize = 0;

	if (ImageFormats::IsCompressedFormat(format)) {
		unsigned int bytesPerblock = ImageFormats::GetBytesPerBlock(format);

		for (unsigned int i = 0; i < mipMapCount; i++) {
			unsigned int w = ((currentMipMapWidth + 3) >> 2);
			unsigned int h = ((currentMipMapHeight + 3) >> 2);

			accumalatedSize += (bytesPerblock * w * h);
			currentMipMapWidth = currentMipMapWidth >> 1;
			currentMipMapHeight = currentMipMapHeight >> 1;
		}
	} else {
		unsigned int bytesPerPixel = ImageFormats::GetBytesPerPixel(format);

		for (unsigned int i = 0; i < mipMapCount; i++) {
			accumalatedSize += (bytesPerPixel * currentMipMapWidth * currentMipMapHeight);
			currentMipMapWidth = currentMipMapWidth >> 1;
			currentMipMapHeight = currentMipMapHeight >> 1;
		}
	}

	return accumalatedSize;
}


bool Image::LoadImageFromFile(const char * fileName, bool useMipMaps) {
	File file = {};

	file.Open(fileName);
	rsize_t fileSize = file.GetFileSize();
	char* data = (char*)malloc(fileSize);
	file.Read(data, fileSize);

	file.Close();

	CreateDDSFromMemory(data, fileSize, useMipMaps);

	free(data);

	return true;
}

unsigned int Image::GetWidth() const {
	return width;
}

unsigned int Image::GetHeight() const {
	return height;
}

unsigned int Image::GetDepth() const {
	return depths;
}

unsigned int Image::GetMipMapCount() const {
	return mipMapCount;
}

unsigned int Image::GetArraySize() const {
	return arraySize;
}

ImageFormats::Format Image::GetFormat() const {
	return format;
}

unsigned char * Image::GetImageData() const {
	return imageData;
}

bool Image::IsCube() const {
	return depths == 0;
}

bool ImageFormats::IsCompressedFormat(Format format) {
	return format >= ImageFormats::DXT1;
}

unsigned int ImageFormats::GetBytesPerPixel(Format format) {
	switch (format) {
		case ImageFormats::R8:
			return 1;
		case ImageFormats::RG8:
			return 2;
		case ImageFormats::RGB8:
			return 3;
		case ImageFormats::RGBA8:
			return 4;
		case ImageFormats::R16:
			return 2;
		case ImageFormats::RG16:
			return 4;
		case ImageFormats::RGB16:
			return 6;
		case ImageFormats::RGBA16:
			return 8;
		default:
			return 0;
	}
}

unsigned int ImageFormats::GetBytesPerBlock(Format format) {
	switch (format) {
		case ImageFormats::DXT1:
			return 8;
		case ImageFormats::DXT3:
		case ImageFormats::DXT5:
			return 16;
		default:
			return 0;
	}
}

unsigned int ImageFormats::GetSize(Format format) {
	if (IsCompressedFormat(format)) {
		return GetBytesPerBlock(format);
	} else {
		return GetBytesPerPixel(format);
	}
}

unsigned int ImageFormats::GetBlockWidth(Format format) {
	switch (format) {
	case ImageFormats::DXT1:
	case ImageFormats::DXT3:
	case ImageFormats::DXT5:
		return 4;
	default:
		return 1;
	}
}

unsigned int ImageFormats::GetWidth(Format format, unsigned int width) {
	if (IsCompressedFormat(format)) {
		return ((width + 3) >> 2);
	} else {
		return width;
	}
}
