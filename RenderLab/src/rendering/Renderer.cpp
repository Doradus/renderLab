#include "Renderer.h"
#include "ShadowPassVS.h"
#include "PointLightShadowPassVS.h"
#include "PointLightShadowPassGS.h"

Renderer::Renderer() :
	shadowPassVS (nullptr),
	omniDirectionalShadowPassVS (nullptr),
	omniDirectionalShadowPassGS (nullptr),
	objectConstantBuffer(nullptr),
	shadowConstantBuffer(nullptr),
	pixelShaderPerFrameBuffer(nullptr),
	materialBuffer(nullptr),
	omniDirectionalShadowPassVSBuffer(nullptr),
	omniDirectionalShadowPassGSBuffer(nullptr),
	samplerState (nullptr),
	omniDirectionalShadowSampler(nullptr)
{}

Renderer::~Renderer() {
	delete renderingInterface;
	renderingInterface = nullptr;

	delete shadowPassVS;
	shadowPassVS = nullptr;

	delete omniDirectionalShadowPassVS;
	omniDirectionalShadowPassVS = nullptr;

	delete omniDirectionalShadowPassGS;
	omniDirectionalShadowPassGS = nullptr;

	delete objectConstantBuffer;
	objectConstantBuffer = nullptr;

	delete shadowConstantBuffer;
	shadowConstantBuffer = nullptr;

	delete pixelShaderPerFrameBuffer;
	pixelShaderPerFrameBuffer = nullptr;

	delete materialBuffer;
	materialBuffer = nullptr;

	delete omniDirectionalShadowPassVSBuffer;
	omniDirectionalShadowPassVSBuffer = nullptr;

	delete omniDirectionalShadowPassGSBuffer;
	omniDirectionalShadowPassGSBuffer = nullptr;

	delete samplerState;
	samplerState = nullptr;

	delete omniDirectionalShadowSampler;
	omniDirectionalShadowSampler = nullptr;
}

void Renderer::CreateHardwareRenderingInterface(int screenWidth, int screenHeight, HWND mainWindow) {
	renderingInterface = new D3D11RenderingInterface(screenWidth, screenHeight, mainWindow);
	renderingInterface->InitRenderer();

	InitShaders();
	CreateConstantBuffers();
}

void Renderer::CreateConstantBuffers() {
	objectConstantBuffer = renderingInterface->CreateConstantBuffer(sizeof(ObjectProperties));
	shadowConstantBuffer = renderingInterface->CreateConstantBuffer(sizeof(VertexShaderShadowResources));
	pixelShaderPerFrameBuffer = renderingInterface->CreateConstantBuffer(sizeof(PixelShaderPerFrameResource));
	materialBuffer = renderingInterface->CreateConstantBuffer(sizeof(MaterialResource));
	omniDirectionalShadowPassVSBuffer = renderingInterface->CreateConstantBuffer(sizeof(OminDirectionalShadowPassVSResources));
	omniDirectionalShadowPassGSBuffer = renderingInterface->CreateConstantBuffer(sizeof(OmniDirectionalShadowPassGSResources));

	renderingInterface->SetVSConstantBuffer(objectConstantBuffer, 0);
	renderingInterface->SetVSConstantBuffer(shadowConstantBuffer, 1);
	renderingInterface->SetVSConstantBuffer(omniDirectionalShadowPassVSBuffer, 2);

	renderingInterface->SetGSConstantBuffer(omniDirectionalShadowPassGSBuffer, 0);

	renderingInterface->SetPSConstantBuffer(pixelShaderPerFrameBuffer, 0);
	renderingInterface->SetPSConstantBuffer(materialBuffer, 1);
}

void Renderer::RenderWorld(World* world) const {
	// render the shadows
	renderingInterface->StartFrame();
	
	PointLightComponent* pointLight = world->GetAllPointLights()[0];
	SpotLightComponent* spotLight = world->GetAllSpotLights()[0];
	//DirectionalLightComponent* directionalLight = world->GetAllDirectionalLights()[0];

	ShadowInfo shadowInfoCollection [2];

	ShadowInfo spotShadow;
	ShadowInfo pointShadow;

	spotShadow.CreateShadowInfo(spotLight);
	pointShadow.CreateShadowInfo(pointLight);

	shadowInfoCollection[0] = spotShadow;
	shadowInfoCollection[1] = pointShadow;

	for (int i = 0; i < 2; i++) {
		if (shadowInfoCollection[i].GetIsOmniDirectionalShadow()) {
			RenderProjectedOmniDirectionalShadow(world, shadowInfoCollection[i]);
		}

		if (shadowInfoCollection[i].GetIsProjectedShadow()) {
			RenderProjectedShadow(world, shadowInfoCollection[i]);
		}
	}


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
	renderingInterface->UpdateConstantBuffer(pixelShaderPerFrameBuffer, &pixelShaderPerFrameResource, sizeof(PixelShaderPerFrameResource));

	const CameraComponent* camera = world->GetActiveCamera();

	XMMATRIX view = XMLoadFloat4x4(&camera->GetCameraView());
	XMMATRIX proj = XMLoadFloat4x4(&camera->GetProjection());

	renderingInterface->BindBackBuffer();
	renderingInterface->SetViewPort(0.0f, 0.0f, 0.0f, 1280.0f, 720.0f, 1.0f);
	renderingInterface->ClearActiveRenderTarget();
	renderingInterface->SetDeafultRasterState();

	XMMATRIX lightView = XMMatrixIdentity();
	XMMATRIX lightProjection = XMMatrixIdentity();

	for (int i = 0; i < 2; i++) {
		if (shadowInfoCollection[i].GetIsOmniDirectionalShadow()) {
			renderingInterface->SetSamplerState(omniDirectionalShadowSampler, 1);
			renderingInterface->SetShaderResources(world->GetShadowMapCube(), 1);
		}

		if (shadowInfoCollection[i].GetIsProjectedShadow()) {
			lightView = XMLoadFloat4x4(&shadowInfoCollection[i].GetShadowViewMatrix());
			lightProjection = XMLoadFloat4x4(&shadowInfoCollection[i].GetShadowViewProjectionMatrix());
			renderingInterface->SetSamplerState(samplerState, 0);
			renderingInterface->SetShaderResources(world->GetShadowMap(), 0);
		}
	}

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

		XMMATRIX lwvp = world * lightView * lightProjection;
		XMMATRIX transposedLightWvp = XMMatrixTranspose(lwvp);
		XMFLOAT4X4 lwvpData;
		XMStoreFloat4x4(&lwvpData, transposedLightWvp);

		ObjectProperties properties;
		properties.wvp = wvpData;
		properties.worldInverse = worldInverseData;
		properties.lightWVP = lwvpData;
		XMStoreFloat4x4(&properties.world, worldTranspose);

		MaterialResource materialResource;
		materialResource.albedo = mesh->GetMaterial()->GetAlbedo();
		materialResource.specularPower = mesh->GetMaterial()->GetSpecularPower();
		materialResource.specularColor = mesh->GetMaterial()->GetSpecularColor();
		materialResource.padding = 0.0f;

		renderingInterface->UpdateConstantBuffer(objectConstantBuffer, &properties, sizeof(ObjectProperties));
		renderingInterface->UpdateConstantBuffer(materialBuffer, &materialResource, sizeof(MaterialResource));
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

void Renderer::InitShaders() {
	size_t size = sizeof(g_shadow_pass_vs);
	shadowPassVS = CreateVertexShader(g_shadow_pass_vs, size);

	size = sizeof(g_point_light_shadow_pass_vs);
	omniDirectionalShadowPassVS = CreateVertexShader(g_point_light_shadow_pass_vs, size);

	size = sizeof(g_point_light_shadow_pass_gs);
	omniDirectionalShadowPassGS = renderingInterface->CreateGeometryShader(g_point_light_shadow_pass_gs, size);

	SamplerConfig samplerConfig = {};

	samplerConfig.addressModeU = BORDER;
	samplerConfig.addressModeV = BORDER;
	samplerConfig.addressModeW = BORDER;
	samplerConfig.filter = COMPARE_BILINEAR_FILTERING;
	samplerConfig.comparisonFunction = LESS_OR_EQUAL;
	
	SamplerConfig omniDirectionalSamplerConfig = {};

	omniDirectionalSamplerConfig.addressModeU = WRAP;
	omniDirectionalSamplerConfig.addressModeV = WRAP;
	omniDirectionalSamplerConfig.addressModeW = WRAP;
	omniDirectionalSamplerConfig.filter = TRILINEAR_FILTERING;
	

	samplerState = renderingInterface->CreateSamplerState(samplerConfig);
	omniDirectionalShadowSampler = renderingInterface->CreateSamplerState(omniDirectionalSamplerConfig);
}

void Renderer::RenderProjectedOmniDirectionalShadow(World * world, ShadowInfo & shadowInfo) const {
	const std::vector<XMFLOAT4X4> shadowViewMatrices = shadowInfo.GetShadowViewProjectionMatrices();
	OmniDirectionalShadowPassGSResources gsResources = {};

	gsResources.lightVPMatrix[0] = shadowViewMatrices[0];
	gsResources.lightVPMatrix[1] = shadowViewMatrices[1];
	gsResources.lightVPMatrix[2] = shadowViewMatrices[2];
	gsResources.lightVPMatrix[3] = shadowViewMatrices[3];
	gsResources.lightVPMatrix[4] = shadowViewMatrices[4];
	gsResources.lightVPMatrix[5] = shadowViewMatrices[5];

	renderingInterface->ClearShaderResource(1);
	renderingInterface->SetRenderTarget(nullptr, world->GetShadowMapCube());
	renderingInterface->SetViewPort(0.0f, 0.0f, 0.0f, 1024.0f, 1024.0f, 1.0f);
	renderingInterface->ClearActiveRenderTarget();
	renderingInterface->SetShadowRasterState();
	renderingInterface->UpdateConstantBuffer(omniDirectionalShadowPassGSBuffer, &gsResources, sizeof(OmniDirectionalShadowPassGSResources));

	for (const StaticMesh* mesh : world->GetAllStaticMeshes()) {
		XMMATRIX world = XMLoadFloat4x4(&mesh->GetWorld());
		XMMATRIX transposedWorld = XMMatrixTranspose(world);
		XMFLOAT4X4 worldData;
		XMStoreFloat4x4(&worldData, transposedWorld);

		OminDirectionalShadowPassVSResources vsResources = {};
		vsResources.worldMatrix = worldData;
		renderingInterface->UpdateConstantBuffer(omniDirectionalShadowPassVSBuffer, &vsResources, sizeof(OminDirectionalShadowPassVSResources));
		renderingInterface->SetGeometryShader(omniDirectionalShadowPassGS);

		renderingInterface->Draw(mesh->GetRenderData(), omniDirectionalShadowPassVS, nullptr);
	}

	renderingInterface->SetGeometryShader(nullptr);
}

void Renderer::RenderProjectedShadow(World * world, ShadowInfo & shadowInfo) const {
	XMMATRIX lightView = XMLoadFloat4x4(&shadowInfo.GetShadowViewMatrix());
	XMMATRIX lightProjection = XMLoadFloat4x4(&shadowInfo.GetShadowViewProjectionMatrix());

	renderingInterface->ClearShaderResource(0);
	renderingInterface->SetRenderTarget(nullptr, world->GetShadowMap());
	renderingInterface->SetViewPort(0.0f, 0.0f, 0.0f, 1024.0f, 1024.0f, 1.0f);
	renderingInterface->ClearActiveRenderTarget();
	renderingInterface->SetShadowRasterState();

	for (const StaticMesh* mesh : world->GetAllStaticMeshes()) {
		XMMATRIX world = XMLoadFloat4x4(&mesh->GetWorld());
		XMMATRIX wvp = world * lightView * lightProjection;
		XMMATRIX transposedWvp = XMMatrixTranspose(wvp);
		XMFLOAT4X4 wvpData;
		XMStoreFloat4x4(&wvpData, transposedWvp);

		VertexShaderShadowResources properties;
		properties.lightWorldViewProj = wvpData;

		renderingInterface->UpdateConstantBuffer(shadowConstantBuffer, &properties, sizeof(VertexShaderShadowResources));
		renderingInterface->Draw(mesh->GetRenderData(), shadowPassVS, nullptr);
	}
}
