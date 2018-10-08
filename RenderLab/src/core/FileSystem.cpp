#include "FileSystem.h"
#include <Windows.h>
File::File() :
fileHandle(nullptr) 
{
}

File::~File() {
}

void File::Open(const char * inFileName) {
	fileName = inFileName;

	if (fileName.size() == 0) {
		MessageBox(0, L"Cannot load a file with an empty name", 0, 0);
		return;
	}

	fopen_s(&fileHandle, inFileName, "r");

	if (!fileHandle) {
		MessageBox(0, L"Could not open file", 0, 0);
		return;
	}

	isOpen = true;
	size = GetFileSize();
}

void File::Close() {
	if (fileHandle) {
		fclose(fileHandle);
		isOpen = false;
		size = 0;
		fileHandle = nullptr;
	}
}

size_t File::Read(void * buffer, size_t byteCount) {
	return fread(buffer, 1, byteCount, fileHandle);
}

size_t File::GetFileSize() const {
	long currentPosition = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_END);
	size_t fileSize= ftell(fileHandle);
	fseek(fileHandle, currentPosition, SEEK_SET);

	return fileSize;
}

MemoryBuffer::~MemoryBuffer() {
}
