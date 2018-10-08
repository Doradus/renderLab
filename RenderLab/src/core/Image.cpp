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
		delete imageData;
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
		//parse dx10 header
	}

	switch (header.pixelFormat.fourCC) {
	case MAKE_CHAR4('D', 'X', 'T', '1') :
		MessageBox(0, L"File is DXT1", 0, 0);
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '2'):
		MessageBox(0, L"File is DXT2", 0, 0);
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '3'):
		MessageBox(0, L"File is DXT3", 0, 0);
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '4'):
		MessageBox(0, L"File is DXT4", 0, 0);
		break;				   
	case MAKE_CHAR4('D', 'X', 'T', '5'):
		MessageBox(0, L"File is DXT5", 0, 0);
		break;
	default:
		MessageBox(0, L"File is other format", 0, 0);
		break;
	}

	return true;
}

bool Image::LoadImageFromFile(const char * fileName, bool useMipMaps) {
	File file = {};

	file.Open(fileName);
	rsize_t fileSize = file.GetFileSize();
	char* data = (char*)malloc(fileSize);
	file.Read(data, fileSize);

	printf("Data: %s\n", data);

	file.Close();

	CreateDDSFromMemory(data, fileSize, useMipMaps);

	free(data);

	return true;
}


