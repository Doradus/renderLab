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
	//read header
	DDS_HEADER header = {};

	size_t copySize = sizeof(header);

	unsigned char* srcPointer = (unsigned char*)&data[0];
	unsigned char* destPointer = (unsigned char*)&header;

	while (copySize >= sizeof(unsigned int)) {
		*((unsigned int*)destPointer) = *((unsigned int*)srcPointer);
		srcPointer += sizeof(unsigned int);
		destPointer += sizeof(unsigned int);
		copySize -= sizeof(unsigned int);
	}

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

	int imageDataSize = (width * height) * 3;
	imageData = (unsigned char*) malloc(sizeof(unsigned char) * imageDataSize);
	unsigned char* destPointer2 = (unsigned char*)imageData;

	while (imageDataSize >= sizeof(unsigned int)) {
		*((unsigned int*)destPointer2) = *((unsigned int*)srcPointer);
		srcPointer += sizeof(unsigned int);
		destPointer2 += sizeof(unsigned int);
		imageDataSize -= sizeof(unsigned int);
	}

	return true;
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

ImageFormats::Format Image::GetFormat() const {
	return format;
}

unsigned char * Image::GetImageData() const {
	return imageData;
}


