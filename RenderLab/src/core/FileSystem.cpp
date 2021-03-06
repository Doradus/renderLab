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
		position = 0;
		fileHandle = nullptr;
	}
}

size_t File::Read(void * buffer, size_t byteCount) {
	position += byteCount;
	return fread(buffer, 1, byteCount, fileHandle);
}

std::string File::ReadLine() {
	std::string line;

	while (!IsEof())
	{
		char c = ReadByte();
		if (c == 10)
			break;
		if (c == 13)
		{
			// Peek next char to see if it's 10, and skip it too
			if (!IsEof())
			{
				char next = ReadByte();
				if (next != 10)
					position -= 1;
			}
			break;
		}

		line.push_back(c);
	}

	return line;;
}

unsigned char File::ReadByte() {
	unsigned char ret;
	Read(&ret, sizeof ret);
	return ret;
}

size_t File::GetFileSize() const {
	long currentPosition = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_END);
	size_t fileSize= ftell(fileHandle);
	fseek(fileHandle, currentPosition, SEEK_SET);

	return fileSize;
}

MemoryBuffer::MemoryBuffer(const void* data):
	buffer((unsigned char*) data),
	position(0)
{}

MemoryBuffer::~MemoryBuffer() {
}

unsigned int MemoryBuffer::Read(void* dest, unsigned int size) {
	unsigned char* srcPointer = (unsigned char*)&buffer[position];
	unsigned char* destPointer = (unsigned char*)dest;

	position += size;

	unsigned int copySize = size;

	/*
	while (copySize >= sizeof(unsigned char)) {
		*((unsigned char*)destPointer) = *((unsigned char*)srcPointer);
		srcPointer += sizeof(unsigned char);
		destPointer += sizeof(unsigned char);
		copySize -= sizeof(unsigned char);
	}
	*/
	

	std::memcpy(destPointer, srcPointer, copySize);

	return size;
}
