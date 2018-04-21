#include "RenderLab.h"
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
RenderLab::RenderLab(HWND window) :
	windowHandle(window) {
};

RenderLab::~RenderLab() {
	delete renderingInterface;
	renderingInterface = nullptr;
};

bool RenderLab::InitLab() {
	if (!CreateRenderingInterface()) {
		return false;
	}

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

	delete vertextShader;
	delete pixelShader;
}

void RenderLab::ShutDown() {

}
