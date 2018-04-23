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
};

bool RenderLab::InitLab() {
	if (!CreateRenderingInterface()) {
		return false;
	}

	BuildGeometry();
	InitShaders();

	return true;
};

bool RenderLab::CreateRenderingInterface() {
	renderingInterface = new D3D11RenderingInterface(500, 500, windowHandle);
	renderingInterface->InitRenderer();
	return true;
}

void RenderLab::InitShaders() {
	size_t size = sizeof(g_basic_vs);
	VertexShader* vertextShader  = renderingInterface->CreateVertexShader(g_basic_vs, size);

	size_t pixelShaderSize = sizeof(g_ps);
	PixelShader* pixelShader = renderingInterface->CreatePixelShader(g_ps, pixelShaderSize);

	renderingInterface->CreateInputLayout(g_basic_vs, size);

	delete vertextShader;
	delete pixelShader;
}

void RenderLab::BuildGeometry() {
	PrimitiveFactory* factory = new PrimitiveFactory();

	MeshData box;

	factory->CreateBox(10, 10, 10, box);

	size_t vertSize = sizeof(box.vertices);
	vertexBuffer = renderingInterface->CreateVertexBuffer(vertSize, &box.vertices[0]);

	size_t indSize = sizeof(box.indices);
	indexBuffer = renderingInterface->CreateIndexBuffer(indSize, &box.indices[0]);

	renderingInterface->CreateConstantBuffer();

	delete factory;
}

void RenderLab::ShutDown() {

}
