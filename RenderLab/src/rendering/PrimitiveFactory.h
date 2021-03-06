#pragma once
#include "RenderingResources.h"

class PrimitiveFactory {
public:
	PrimitiveFactory();
	~PrimitiveFactory();

	void CreateBox(unsigned int width, unsigned int height, unsigned int depth, MeshData& meshDataOut) const;
	void CreatePlane(float width, float depth, unsigned int subDivsWidth, unsigned int subDivsHeight, MeshData& meshDataOut) const;
	void CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount, MeshData& meshDataOut) const;

private:

};