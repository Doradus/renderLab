#include "Renderer.h"
#include "ShadowPassVS.h"
#include "PointLightShadowPassVS.h"
#include "PointLightShadowPassGS.h"

Renderer::Renderer() :
	shadowPassVS(nullptr),
	omniDirectionalShadowPassVS(nullptr),
	omniDirectionalShadowPassGS(nullptr),
	objectConstantBuffer(nullptr),
	shadowConstantBuffer(nullptr),
	pixelShaderPerFrameBuffer(nullptr),
	materialBuffer(nullptr),
	omniDirectionalShadowPassVSBuffer(nullptr),
	omniDirectionalShadowPassGSBuffer(nullptr),
	samplerState(nullptr),
	omniDirectionalShadowSampler(nullptr),
	textureSampler(nullptr),
	shadowMap(nullptr),
	shadowMapCube(nullptr),
	diffuseMap(nullptr),
	lightSpaceTransformBuffer(nullptr)
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

	delete textureSampler;
	textureSampler = nullptr;

	delete lightSpaceTransformBuffer;
	lightSpaceTransformBuffer = nullptr;

	if (shadowMap) {
		delete shadowMap;
		shadowMap = nullptr;
	}

	if (shadowMapCube) {
		delete shadowMapCube;
		shadowMapCube = nullptr;
	}

	if (diffuseMap) {
		delete diffuseMap;
		diffuseMap = nullptr;
	}
}

void Renderer::CreateHardwareRenderingInterface(int screenWidth, int screenHeight, HWND mainWindow) {
	renderingInterface = new D3D11RenderingInterface(screenWidth, screenHeight, mainWindow);
	renderingInterface->InitRenderer();

	InitTextureResources();
	InitShaders();
	CreateConstantBuffers();
}

void Renderer::AllocateShadowRenderTargets(World * world) {
	std::vector<PointLightComponent*> shadowCastingPointLights;
	std::vector<DirectionalLightComponent*> shadowCastingDirectionalLights;
	std::vector<SpotLightComponent*> shadowCastingSpotLights;

	for (PointLightComponent* light : world->GetAllPointLights()) {
		if (light->GetCastsShadows()) {
			shadowCastingPointLights.push_back(light);
		}
	}

	for (DirectionalLightComponent* light : world->GetAllDirectionalLights()) {
		if (light->GetCastsShadows()) {
			shadowCastingDirectionalLights.push_back(light);
		}
	}

	for (SpotLightComponent* light : world->GetAllSpotLights()) {
		if (light->GetCastsShadows()) {
			shadowCastingSpotLights.push_back(light);
		}
	}

	const unsigned int numberOfOmniDirectionalShadows = shadowCastingPointLights.size();
	
	if (numberOfOmniDirectionalShadows > 0) {
		shadowMapCube = renderingInterface->CreateTexture2d(1024, 1024, 6, true, false, 1, ImageFormats::SHADOW_DEPTH, TextureBindAsDepthStencil | TextureBindAsShaderResource, 1, nullptr);
	}

	const unsigned int numberOfDirectionalAndSpotLightShadows = shadowCastingSpotLights.size() + shadowCastingDirectionalLights.size();

	if (numberOfDirectionalAndSpotLightShadows > 0) {
		shadowMap = renderingInterface->CreateTexture2d(1024, 1024, 2, false, true, 1, ImageFormats::SHADOW_DEPTH, TextureBindAsDepthStencil | TextureBindAsShaderResource | CreateRTVArraySlicesIndividualy,  1, nullptr);
	}

	for (unsigned int i = 0; i < shadowCastingSpotLights.size(); i++) {
		shadowCastingSpotLights[i]->SetShadowMapId(i);
	}

	for (unsigned int i = shadowCastingSpotLights.size(); i < numberOfDirectionalAndSpotLightShadows; i++) {
		shadowCastingDirectionalLights[i]->SetShadowMapId(i);
	}
}

void Renderer::CreateConstantBuffers() {
	objectConstantBuffer = renderingInterface->CreateConstantBuffer(sizeof(ObjectProperties));
	shadowConstantBuffer = renderingInterface->CreateConstantBuffer(sizeof(VertexShaderShadowResources));
	pixelShaderPerFrameBuffer = renderingInterface->CreateConstantBuffer(sizeof(PixelShaderPerFrameResource));
	materialBuffer = renderingInterface->CreateConstantBuffer(sizeof(MaterialResource));
	omniDirectionalShadowPassVSBuffer = renderingInterface->CreateConstantBuffer(sizeof(OminDirectionalShadowPassVSResources));
	omniDirectionalShadowPassGSBuffer = renderingInterface->CreateConstantBuffer(sizeof(OmniDirectionalShadowPassGSResources));
	lightSpaceTransformBuffer = renderingInterface->CreateConstantBuffer(sizeof(LightSpaceTransformBuffer));

	renderingInterface->SetVSConstantBuffer(objectConstantBuffer, 0);
	renderingInterface->SetVSConstantBuffer(shadowConstantBuffer, 1);
	renderingInterface->SetVSConstantBuffer(omniDirectionalShadowPassVSBuffer, 2);
	renderingInterface->SetVSConstantBuffer(lightSpaceTransformBuffer, 3);

	renderingInterface->SetGSConstantBuffer(omniDirectionalShadowPassGSBuffer, 0);

	renderingInterface->SetPSConstantBuffer(pixelShaderPerFrameBuffer, 0);
	renderingInterface->SetPSConstantBuffer(materialBuffer, 1);
}

void Renderer::RenderWorld(World* world) const {
	// render the shadows
	renderingInterface->StartFrame();
	
	//create shadow info

	std::vector<ShadowInfo> allShadowInfo;

	for (PointLightComponent* light : world->GetAllPointLights()) {
		if (light->GetCastsShadows()) {
			ShadowInfo shadowInfo;
			shadowInfo.CreateShadowInfo(light);
			allShadowInfo.push_back(shadowInfo);
		}
	}

	for (SpotLightComponent* light : world->GetAllSpotLights()) {
		if (light->GetCastsShadows()) {
			ShadowInfo shadowInfo;
			shadowInfo.CreateShadowInfo(light);
			allShadowInfo.push_back(shadowInfo);
		}
	}

	for (ShadowInfo info : allShadowInfo) {
		if (info.GetIsOmniDirectionalShadow()) {
			RenderProjectedOmniDirectionalShadow(world, info);
		}

		if (info.GetIsProjectedShadow()) {
			RenderProjectedShadow(world, info);
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
		lightResource.useShadow = light->GetCastsShadows();
		lightResource.type = 0;
		lightResource.shadowMapId = float(light->GetShadowMapId());
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
		lightResource.useShadow = light->GetCastsShadows();
		lightResource.type = 1;
		lightResource.shadowMapId = float(light->GetShadowMapId());
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
		lightResource.useShadow = light->GetCastsShadows();
		lightResource.shadowMapId = float(light->GetShadowMapId());
		pixelShaderPerFrameResource.lightResources[lightIndex++] = lightResource;
	}

	pixelShaderPerFrameResource.cameraPosition = world->GetActiveCamera()->GetPosition();
	renderingInterface->UpdateConstantBuffer(pixelShaderPerFrameBuffer, &pixelShaderPerFrameResource, sizeof(PixelShaderPerFrameResource));

	const CameraComponent* camera = world->GetActiveCamera();

	XMMATRIX view = XMLoadFloat4x4(&camera->GetCameraView());
	XMMATRIX proj = XMLoadFloat4x4(&camera->GetProjection());

	renderingInterface->BindBackBuffer();
	renderingInterface->SetViewPort(0.0f, 0.0f, 0.0f, 1920.0f, 1080.0f, 1.0f);
	renderingInterface->ClearActiveRenderTarget();
	renderingInterface->SetDeafultRasterState();

	LightSpaceTransformBuffer lightSpaceData = {};

	for (ShadowInfo info : allShadowInfo) {
		if (info.GetIsOmniDirectionalShadow()) {
			renderingInterface->SetSamplerState(omniDirectionalShadowSampler, 2);
			renderingInterface->SetShaderResources(shadowMapCube, 2);
		}

		if (info.GetIsProjectedShadow()) {
			XMMATRIX lightView = XMLoadFloat4x4(&info.GetShadowViewMatrix());
			XMMATRIX lightProjection = XMLoadFloat4x4(&info.GetShadowViewProjectionMatrix());

			XMMATRIX lwvp = lightView * lightProjection;
			XMMATRIX transposedLightWvp = XMMatrixTranspose(lwvp);
			XMFLOAT4X4 lwvpData;
			XMStoreFloat4x4(&lwvpData, transposedLightWvp);
			lightSpaceData.lightViewProjection[info.GetShadowId()] = lwvpData;

			renderingInterface->SetSamplerState(samplerState, 1);
			renderingInterface->SetShaderResources(shadowMap, 1);
		}
	}

	renderingInterface->SetSamplerState(textureSampler, 0);
	renderingInterface->SetShaderResources(diffuseMap, 0);
	renderingInterface->UpdateConstantBuffer(lightSpaceTransformBuffer, &lightSpaceData, sizeof(LightSpaceTransformBuffer));

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

void Renderer::InitTextureResources() {
	Image image = {};
	image.LoadImageFromFile("floor_rgb.dds", true);

	diffuseMap = renderingInterface->CreateTexture2d(image.GetWidth(), image.GetHeight(), 1, false, false, image.GetMipMapCount(), image.GetFormat(), TextureBindAsShaderResource, 1, image.GetImageData());
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

	SamplerConfig defaultSampler = {};

	defaultSampler.addressModeU = WRAP;
	defaultSampler.addressModeV = WRAP;
	defaultSampler.addressModeW = WRAP;
	defaultSampler.filter = TRILINEAR_FILTERING;
	//defaultSampler.maxAnisotropy = 6;
	defaultSampler.minLOD = 0.0f;
	defaultSampler.maxLOD = 11.0f;
	defaultSampler.mipLODBias = 0.0f;
	
	textureSampler = renderingInterface->CreateSamplerState(defaultSampler);;
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

	renderingInterface->ClearShaderResource(2);
	RenderTargetInfo* projectedShadowInfo = new RenderTargetInfo(shadowMapCube, 0);
	RenderTargetInfo* projectedShadowInfoDepth = new RenderTargetInfo(shadowMapCube, 0);
	renderingInterface->SetRenderTarget(1, projectedShadowInfo, projectedShadowInfoDepth);
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

	delete projectedShadowInfo;
	delete projectedShadowInfoDepth;
	renderingInterface->SetGeometryShader(nullptr);
}

void Renderer::RenderProjectedShadow(World * world, ShadowInfo & shadowInfo) const {
	XMMATRIX lightView = XMLoadFloat4x4(&shadowInfo.GetShadowViewMatrix());
	XMMATRIX lightProjection = XMLoadFloat4x4(&shadowInfo.GetShadowViewProjectionMatrix());

	renderingInterface->ClearShaderResource(1);

	RenderTargetInfo* projectedShadowInfo = new RenderTargetInfo(shadowMap, 0);
	RenderTargetInfo* projectedShadowInfoDepth = new RenderTargetInfo(shadowMap, shadowInfo.GetShadowId());
	renderingInterface->SetRenderTarget(1, projectedShadowInfo, projectedShadowInfoDepth);
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

	delete projectedShadowInfo;
	delete projectedShadowInfoDepth;
}
