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

void Renderer::RenderWorld(const World* world) const {
	//update the start frame const buffer

	DirectionalLightResource lightResourceStruct;
	
	for (const DirectionalLightComponent* light : world->GetAllDirectionalLights()) {
		lightResourceStruct.position = light->GetPosition();
		lightResourceStruct.enabled = light->GetIsEnabled();
		lightResourceStruct.color = light->GetLightColor();
		lightResourceStruct.direction = light->GetDirection();
		lightResourceStruct.padding1 = 0.0f;
		lightResourceStruct.padding2 = 0.0f;
	}

	renderingInterface->ConstantBuffersFrameStart(lightResourceStruct);

	const CameraComponent* camera = world->GetActiveCamera();

	XMMATRIX view = XMLoadFloat4x4(&camera->GetCameraView());
	XMMATRIX proj = XMLoadFloat4x4(&camera->GetProjection());

	for (const StaticMesh* mesh : world->GetAllStaticMeshes()) {
		XMMATRIX world = XMLoadFloat4x4(&mesh->GetWorld());
		XMMATRIX wvp = world * view * proj;
		XMMATRIX transposedWvp = XMMatrixTranspose(wvp);
		XMFLOAT4X4 wvpData;
		XMStoreFloat4x4(&wvpData, transposedWvp);


		XMMATRIX worldCopy = world;
		worldCopy.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(worldCopy);
		XMMATRIX worldInverse = XMMatrixInverse(&det, worldCopy);
		XMFLOAT4X4 worldInverseData;
		XMStoreFloat4x4(&worldInverseData, worldInverse);

		ObjectProperties properties;
		properties.wvp = wvpData;
		properties.worldInverse = worldInverseData;

		renderingInterface->ConstantBuffersMiddFrame(properties);
		RenderPrimitive(mesh);
	}
}

void Renderer::RenderPrimitive(const StaticMesh * mesh) const {
	renderingInterface->Draw(mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetVertexShader(), mesh->GetPixelShader());
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
