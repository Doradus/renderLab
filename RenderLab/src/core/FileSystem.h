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
	MemoryBuffer(const void* data, size_t size);
	~MemoryBuffer();

public: 
	//buffer
};