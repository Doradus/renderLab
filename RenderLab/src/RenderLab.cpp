#include "RenderLab.h"
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
RenderLab::RenderLab(HWND window) :
	windowHandle(window) {
};

RenderLab::~RenderLab() {
	delete renderingInterface;
	renderingInterface = nullptr;

	delete vertexBuffer;
	vertexBuffer = nullptr;
	delete indexBuffer;
	indexBuffer = nullptr;

	delete vertextShader;
	delete pixelShader;
};

bool RenderLab::InitLab() {
	if (!CreateRenderingInterface()) {
		return false;
	}

	BuildGeometry();
	InitShaders();
	Draw();

	return true;
};

bool RenderLab::CreateRenderingInterface() {
	renderingInterface = new D3D11RenderingInterface(1280, 720, windowHandle);
	renderingInterface->InitRenderer();
	return true;
}

void RenderLab::InitShaders() {
	size_t size = sizeof(g_basic_vs);
	vertextShader  = renderingInterface->CreateVertexShader(g_basic_vs, size);

	size_t pixelShaderSize = sizeof(g_ps);
	pixelShader = renderingInterface->CreatePixelShader(g_ps, pixelShaderSize);

	renderingInterface->CreateInputLayout(g_basic_vs, size);
}

void RenderLab::BuildGeometry() {
	PrimitiveFactory* factory = new PrimitiveFactory();

	MeshData box;

	factory->CreateBox(1, 1, 1, box);

	size_t vertSize = sizeof(Vertex) * box.vertices.size();
	vertexBuffer = renderingInterface->CreateVertexBuffer(vertSize, &box.vertices[0]);

	size_t indSize = sizeof(unsigned int) * box.indices.size();
	indexBuffer = renderingInterface->CreateIndexBuffer(indSize, &box.indices[0]);

	renderingInterface->CreateConstantBuffer();

	delete factory;
}

void RenderLab::Draw() {
	renderingInterface->Draw(vertexBuffer, indexBuffer, vertextShader, pixelShader);
}

void RenderLab::ShutDown() {

}
