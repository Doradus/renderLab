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
	samplerState (nullptr) {}

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
	
	/*
	float sceneRadius = 20.0f;
	const XMFLOAT3 sceneCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//generate view matrix
	const DirectionalLightComponent* light = world->GetAllDirectionalLights()[0];

	const XMFLOAT3 lightDirectionStorage = light->GetDirection();
	const XMVECTOR lightDirection = XMLoadFloat3(&lightDirectionStorage);
	const XMVECTOR lightPosition = -2.0f * sceneRadius * lightDirection;
	const XMVECTOR targetPostion = XMLoadFloat3(&sceneCenter);
	const XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	const XMMATRIX lightView = XMMatrixLookAtLH(lightPosition, targetPostion, up);

	//generate projection matrix
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPostion, lightView));
	const float left = sphereCenterLS.x - sceneRadius;
	const float right = sphereCenterLS.x + sceneRadius;
	const float bottom = sphereCenterLS.y - sceneRadius;
	const float top = sphereCenterLS.y + sceneRadius;
	const float nearPlane = sphereCenterLS.z - sceneRadius;
	const float farPlane = sphereCenterLS.z + sceneRadius;

	XMMATRIX lightProj = XMMatrixOrthographicOffCenterLH(left, right, bottom, top, nearPlane, farPlane);
	*/

	//SpotLightComponent* spotLight = world->GetAllSpotLights()[0];
	//spotLight->UpdateViewMatrix();
	//spotLight->UpdateProjectionMatrix();

	//XMMATRIX lightView = XMLoadFloat4x4(&spotLight->GetViewMatrix());
	//XMMATRIX lightProj = XMLoadFloat4x4(&spotLight->GetProjectionMatrix());

	PointLightComponent* pointLight = world->GetAllPointLights()[0];

	ShadowInfo shadowInfo;
	shadowInfo.CreateShadowInfo(pointLight);

	if (shadowInfo.GetIsOmniDirectionalShadow()) {
		RenderProjectedOmniDirectionalShadow(world, shadowInfo);
	}

	/*
	for (const StaticMesh* mesh : world->GetAllStaticMeshes()) {
		XMMATRIX world = XMLoadFloat4x4(&mesh->GetWorld());
		XMMATRIX transposedWorld = XMMatrixTranspose(world);
		XMFLOAT4X4 worldData;
		XMStoreFloat4x4(&worldData, transposedWorld);

		OminDirectionalShadowPassVSResources vsResources = {};
		vsResources.worldMatrix = worldData;

		XMMATRIX wvp = world * lightView * lightProj;
		XMMATRIX transposedWvp = XMMatrixTranspose(wvp);
		XMFLOAT4X4 wvpData;
		XMStoreFloat4x4(&wvpData, transposedWvp);

		VertexShaderShadowResources properties;
		properties.lightWorldViewProj = wvpData;

		renderingInterface->UpdateConstantBuffer(shadowConstantBuffer, &properties, sizeof(VertexShaderShadowResources));
		renderingInterface->Draw(mesh->GetRenderData(), shadowPassVS, nullptr);
	}
	*/

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

	renderingInterface->SetSamplerState(samplerState);
	renderingInterface->SetShaderResources(world->GetShadowMapCube());

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

		//XMMATRIX lwvp = world * lightView * lightProj;
		XMMATRIX lwvp = world;
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
	/*
	samplerConfig.addressModeU = BORDER;
	samplerConfig.addressModeV = BORDER;
	samplerConfig.addressModeW = BORDER;
	samplerConfig.filter = COMPARE_BILINEAR_FILTERING;
	samplerConfig.comparisonFunction = LESS_OR_EQUAL;
	*/

	samplerConfig.addressModeU = WRAP;
	samplerConfig.addressModeV = WRAP;
	samplerConfig.addressModeW = WRAP;
	samplerConfig.filter = TRILINEAR_FILTERING;
	//samplerConfig.comparisonFunction = LESS_OR_EQUAL;

	samplerState = renderingInterface->CreateSamplerState(samplerConfig);
}

void Renderer::RenderProjectedOmniDirectionalShadow(World * world, ShadowInfo & shadowInfo) const {
	const std::vector<XMFLOAT4X4> shadowVoewMatrices = shadowInfo.GetShadowViewProjectionMatrices();
	OmniDirectionalShadowPassGSResources gsResources = {};

	gsResources.lightVPMatrix[0] = shadowVoewMatrices[0];
	gsResources.lightVPMatrix[1] = shadowVoewMatrices[1];
	gsResources.lightVPMatrix[2] = shadowVoewMatrices[2];
	gsResources.lightVPMatrix[3] = shadowVoewMatrices[3];
	gsResources.lightVPMatrix[4] = shadowVoewMatrices[4];
	gsResources.lightVPMatrix[5] = shadowVoewMatrices[5];

	renderingInterface->ClearShaderResource();
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

}
