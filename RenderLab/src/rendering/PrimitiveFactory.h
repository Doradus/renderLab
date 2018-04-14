#pragma once
#include "RenderUtils.h"

class PrimitiveFactory {
public:
	PrimitiveFactory();
	~PrimitiveFactory();

	void CreateBox(unsigned int width, unsigned int height, unsigned int depth, MeshData& meshDataOut) const;

private:

};

PrimitiveFactory::PrimitiveFactory()
{
}

PrimitiveFactory::~PrimitiveFactory()
{
}