#include "RenderData.h"

RenderData::RenderData() :
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{}

RenderData::~RenderData() {
}

void RenderData::SetVertexBuffer(VertexBuffer* buffer) {
	vertexBuffer = buffer;
}

void RenderData::SetIndexBuffer(IndexBuffer* buffer) {
	indexBuffer = buffer;
}

void RenderData::SetNumIndices(unsigned int numberOfIndices) {
	numIndices = numberOfIndices;
}

VertexBuffer* RenderData::GetVertexBuffer() const {
	return vertexBuffer;
}

IndexBuffer* RenderData::GetIndexBuffer() const {
	return indexBuffer;
}

unsigned int RenderData::GetNumIndices() const {
	return numIndices;
}

