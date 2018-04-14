
#include "PrimitiveFactory.h"

void PrimitiveFactory::CreateBox(unsigned int width, unsigned int height, unsigned int depth, MeshData& meshDataOut) const {
	Vertex v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	// Fill in the front face vertex data.
	v[0] = Vertex(-w2, -h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);

	// Fill in the top face vertex data.
	v[8] = Vertex(-w2, +h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[9] = Vertex(-w2, +h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[18] = Vertex(-w2, +h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[19] = Vertex(-w2, -h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+w2, -h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[22] = Vertex(+w2, +h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);
	v[23] = Vertex(+w2, -h2, +d2, 0.5f, 0.5f, 0.5f, 1.0f);

	meshDataOut.vertices.assign(&v[0], &v[24]);

	unsigned int indices[36];

	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in in the back face index data
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	meshDataOut.indices.assign(&indices[0], &indices[36]);
}