
#include "PrimitiveFactory.h"

PrimitiveFactory::PrimitiveFactory()
{
}

PrimitiveFactory::~PrimitiveFactory()
{
}

void PrimitiveFactory::CreateBox(unsigned int width, unsigned int height, unsigned int depth, MeshData& meshDataOut) const {
	Vertex v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	// Fill in the front face vertex data.
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(w2, h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[5] = Vertex(w2, -h2, d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[6] = Vertex(w2, h2, d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[7] = Vertex(-w2, h2, d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the top face vertex data.
	v[8] = Vertex(-w2, h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-w2, h2, d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(w2, h2, d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0);
	v[11] = Vertex(w2, h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[13] = Vertex(w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[14] = Vertex(w2, -h2, d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0);
	v[15] = Vertex(-w2, -h2, d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, h2, d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(w2, h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(w2, h2, d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0);
	v[23] = Vertex(w2, -h2, d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshDataOut.vertices.assign(&v[0], &v[24]);

	unsigned int indices[36];

	// fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// fill in the bacik face index data
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// fill in the top face index data
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	meshDataOut.indices.assign(&indices[0], &indices[36]);
}

void PrimitiveFactory::CreatePlane(float width, float depth, unsigned int subDivsWidth, unsigned int subDivsDepth, MeshData & meshDataOut) const {
	unsigned int vertexCount = subDivsWidth * subDivsDepth;
	unsigned int faceCount = (subDivsWidth - 1)*(subDivsDepth - 1) * 2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (subDivsWidth - 1);
	float dz = depth / (subDivsDepth - 1);

	float du = 1.0f / (subDivsWidth - 1);
	float dv = 1.0f / (subDivsDepth - 1);

	meshDataOut.vertices.resize(vertexCount);
	for (unsigned int i = 0; i < subDivsDepth; ++i) {
		float z = halfDepth - i*dz;
		float v = dv * i * 1.5f;
		for (unsigned int j = 0; j < subDivsWidth; ++j) {
			float x = -halfWidth + j*dx;
			float u = du * j * 1.5f;
			
			meshDataOut.vertices[i*subDivsDepth + j].position = XMFLOAT3(x, 0.0f, z);
			meshDataOut.vertices[i*subDivsDepth + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshDataOut.vertices[i*subDivsDepth + j].tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
			meshDataOut.vertices[i*subDivsDepth + j].uv = XMFLOAT2(u, v);
		}
	}

	//
	// Create the indices.
	//

	meshDataOut.indices.resize(faceCount * 3); // 3 indices per face

											// Iterate over each quad and compute indices.
	unsigned int k = 0;
	for (unsigned int i = 0; i < subDivsWidth - 1; ++i) {
		for (unsigned int j = 0; j < subDivsDepth - 1; ++j) {
			meshDataOut.indices[k] = i*subDivsDepth + j;
			meshDataOut.indices[k + 1] = i*subDivsDepth + j + 1;
			meshDataOut.indices[k + 2] = (i + 1)*subDivsDepth + j;

			meshDataOut.indices[k + 3] = (i + 1)*subDivsDepth + j;
			meshDataOut.indices[k + 4] = i*subDivsDepth + j + 1;
			meshDataOut.indices[k + 5] = (i + 1)*subDivsDepth + j + 1;

			k += 6; // next quad
		}
	}
}

void PrimitiveFactory::CreateSphere(float radius, unsigned int sliceCount, unsigned int stackCount, MeshData & meshDataOut) const {
	const Vertex topVertex(0.0f, radius, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	const Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0, 1.0f);

	meshDataOut.vertices.push_back(topVertex);

	const float phiStep = XM_PI / stackCount;
	const float thetaStep = XM_2PI / sliceCount;

	for (unsigned int i = 1; i < stackCount; i++) {
		const float phi = phiStep * i;

		for (unsigned int j = 0; j <= sliceCount; j++) {
			const float theta = thetaStep * j;

			Vertex vertex;
			vertex.position.x = radius * sinf(phi) * cosf(theta);
			vertex.position.y = radius * cosf(phi);
			vertex.position.z = radius * sinf(phi) * sinf(theta);

			XMVECTOR normal = XMLoadFloat3(&vertex.position);
			XMVECTOR normalNormalized = XMVector3Normalize(normal);
			XMStoreFloat3(&vertex.normal, normalNormalized);

			vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
			vertex.tangent.y = 0.0f;
			vertex.tangent.z = radius * sinf(phi) * cosf(theta);

			float u = theta / XM_2PI;
			float v = phi / XM_PI;

			vertex.uv = XMFLOAT2(u, v);

			meshDataOut.vertices.push_back(vertex);
		}
	}

	meshDataOut.vertices.push_back(bottomVertex);

	//Calculate the indices for the vertices connected to the top vertex
	for (unsigned int i = 1; i <= sliceCount; ++i) {
		meshDataOut.indices.push_back(0);
		meshDataOut.indices.push_back(i + 1);
		meshDataOut.indices.push_back(i);
	}

	unsigned int offset = 1;
	unsigned int ringVertex = sliceCount + 1;
	for (unsigned int i = 0; i < stackCount - 2; ++i) {
		for (unsigned int j = 0; j < sliceCount; ++j) {
			meshDataOut.indices.push_back(offset + ringVertex * i + j);
			meshDataOut.indices.push_back(offset + ringVertex * i + j + 1);
			meshDataOut.indices.push_back(offset + ringVertex * (i + 1) + j);

			meshDataOut.indices.push_back(offset + ringVertex * (i + 1) + j);
			meshDataOut.indices.push_back(offset + ringVertex * i + j + 1);
			meshDataOut.indices.push_back(offset + ringVertex * (i + 1) + j + 1);
		}
	}

	//Add the bottom vertex

	//The position of the bottom vertex in the vertices array
	unsigned int bottomVertexIndex = (unsigned int)meshDataOut.vertices.size() - 1;

	//Offset the indices to the first vertex in the last ring
	offset = bottomVertexIndex - ringVertex;

	for (unsigned int i = 0; i < sliceCount; ++i) {
		meshDataOut.indices.push_back(bottomVertexIndex);
		meshDataOut.indices.push_back(offset + i);
		meshDataOut.indices.push_back(offset + i + 1);
	}
}
