#include "Renderer.h"

Renderer::Renderer(): 
renderingInterface (nullptr) {
}

Renderer::~Renderer() {
	delete renderingInterface;
	renderingInterface = nullptr;
}

void Renderer::CreateHardwareRenderingInterface(int screenWidth, int screenHeight, HWND mainWindow) {
	renderingInterface = new D3D11RenderingInterface(screenWidth, screenHeight, mainWindow);
	renderingInterface->InitRenderer();
}

void Renderer::RenderPrimitive(StaticMesh * staticMesh) const {
	renderingInterface->Draw(staticMesh->GetVertexBuffer(), staticMesh->GetIndexBuffer(), staticMesh->GetVertexShader(), staticMesh->GetPixelShader());
}

void Renderer::UpdateConstantBuffer(XMFLOAT4X4 matrix) const {
	renderingInterface->UpdateConstantBuffer(matrix);
}

VertexBuffer* Renderer::CreateVertexBuffer(unsigned int size, const void * data) const {
	return renderingInterface->CreateVertexBuffer(size, data);
}

IndexBuffer * Renderer::CreateIndexBuffer(unsigned int size, const void * data) const {
	return renderingInterface->CreateIndexBuffer(size, data);
}

VertexShader * Renderer::CreateVertexShader(const unsigned char * shaderSource, size_t size) const {
	return renderingInterface->CreateVertexShader(shaderSource, size);
}

PixelShader * Renderer::CreatePixelShader(const unsigned char * shaderSource, size_t size) const {
	return renderingInterface->CreatePixelShader(shaderSource, size);
}

void Renderer::CreateInputLayout(const unsigned char * shaderSource, size_t size) const {
	renderingInterface->CreateInputLayout(shaderSource, size);
}

void Renderer::CreateConstantBuffer() const {
	renderingInterface->CreateConstantBuffer();
}
