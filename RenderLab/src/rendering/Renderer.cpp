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
	PixelShaderPerFrameResource pixelShaderPerFrameResource;
	
	const unsigned int activeLights = world->GetAllDirectionalLights().size() + world->GetAllPointLights().size() + world->GetAllSpotLights().size();
	unsigned int lightIndex = 0;
	pixelShaderPerFrameResource.activeLights = activeLights;

	for (const DirectionalLightComponent* light : world->GetAllDirectionalLights()) {
		LightPropertiesResource lightResource;
		lightResource.enabled = light->GetIsEnabled();
		lightResource.color = light->GetLightColor();
		lightResource.direction = light->GetDirection();
		lightResource.brightness = light->GetBrightness();
		lightResource.type = 0;
		pixelShaderPerFrameResource.lightResources[lightIndex++] = lightResource;
	}

	for (const PointLightComponent* light : world->GetAllPointLights()) {
		LightPropertiesResource lightResource;
		lightResource.enabled = light->GetIsEnabled();
		lightResource.color = light->GetLightColor();
		lightResource.position = light->GetPosition();
		lightResource.range = light->GetRange();
		lightResource.attenuation = light->GetAttenuation();
		lightResource.brightness = light->GetBrightness();
		lightResource.type = 1;
		pixelShaderPerFrameResource.lightResources[lightIndex++] = lightResource;
	}

	for (const SpotLightComponent* light : world->GetAllSpotLights()) {
		LightPropertiesResource lightResource;
		lightResource.enabled = light->GetIsEnabled();
		lightResource.color = light->GetLightColor();
		lightResource.position = light->GetPosition();
		lightResource.range = light->GetRange();
		lightResource.attenuation = light->GetAttenuation();
		lightResource.brightness = light->GetBrightness();
		lightResource.type = 2;
		lightResource.coneAngle = light->GetConeAngleRadians();
		lightResource.penumbraAngle = light->GetPenumbraAngleRadians();
		lightResource.direction = light->GetDirection();
		pixelShaderPerFrameResource.lightResources[lightIndex++] = lightResource;
	}

	pixelShaderPerFrameResource.cameraPosition = world->GetActiveCamera()->GetPosition();
	renderingInterface->ConstantBuffersFrameStart(pixelShaderPerFrameResource);

	const CameraComponent* camera = world->GetActiveCamera();

	XMMATRIX view = XMLoadFloat4x4(&camera->GetCameraView());
	XMMATRIX proj = XMLoadFloat4x4(&camera->GetProjection());

	renderingInterface->StartFrame();

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

		XMMATRIX worldTranspose = XMMatrixTranspose(world);

		ObjectProperties properties;
		properties.wvp = wvpData;
		properties.worldInverse = worldInverseData;
		XMStoreFloat4x4(&properties.world, worldTranspose);

		MaterialResource materialResource;
		materialResource.albedo = mesh->GetMaterial()->GetAlbedo();
		materialResource.specularPower = mesh->GetMaterial()->GetSpecularPower();
		materialResource.specularColor = mesh->GetMaterial()->GetSpecularColor();
		materialResource.padding = 0.0f;

		renderingInterface->ConstantBuffersMiddFrame(properties, materialResource);
		RenderPrimitive(mesh);
	}

	renderingInterface->EndFrame();
}

void Renderer::RenderPrimitive(const StaticMesh * mesh) const {
	renderingInterface->Draw(mesh->GetRenderData(), mesh->GetVertexShader(), mesh->GetPixelShader());
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
