#include "StaticMesh.h"

StaticMesh::StaticMesh() :
vertexBuffer (nullptr),
indexBuffer(nullptr)
{}

StaticMesh::~StaticMesh() {

}

void StaticMesh::SetVertexBuffer(VertexBuffer* buffer) {
	vertexBuffer = buffer;
}

void StaticMesh::SetIndexBuffer(IndexBuffer* buffer) {
	indexBuffer = buffer;
}

VertexBuffer* StaticMesh::GetVertexBuffer() const {
	return vertexBuffer;
}

IndexBuffer* StaticMesh::GetIndexBuffer() const {
	return indexBuffer;
}