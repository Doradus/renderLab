#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>

class File {
public:
	File();
	~File();

	void Open(const char* inFileName);
	void Close();
	size_t Read(void* buffer, size_t byteCount);
	void Write();

	bool GetIsOpen() const;
	size_t GetFileSize() const;
private:
	std::string fileName;
	bool isOpen;
	FILE* fileHandle;
	size_t size;
};

class MemoryBuffer {

public: 
	MemoryBuffer(const void* data);
	~MemoryBuffer();
	unsigned int Read(void* dest, unsigned int size);
	unsigned char* GetBuffer();

private:
	bool readOnly;
	unsigned int position;
	unsigned char* buffer;
};