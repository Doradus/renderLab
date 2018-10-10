#include "D3D11RenderingInterface.h"
#include "MathUtils.h"
#include <cassert>

D3D11RenderingInterface::D3D11RenderingInterface(int screenWidth, int screenHeight, HWND window) :
	screenWidth(screenWidth),
	screenHeight(screenHeight),
	d3dDevice(nullptr),
	d3dImmediateContext(nullptr),
	defaultRenderTargetView(nullptr),
	backBuffer(nullptr),
	backBufferDepthStencil(nullptr),
	swapChain(nullptr),
	defaultDepthStencilBuffer(nullptr),
	windowHandle(window),
	enable4xMsaa(true),
	xmsaaQuality(0),
	stateCache(nullptr),
	shadowState(nullptr),
	defaultState(nullptr),
	driverType(D3D_DRIVER_TYPE_HARDWARE)
{
	GTextureFormatInfo[UNKNOWN].platformFormat = DXGI_FORMAT_UNKNOWN;
	GTextureFormatInfo[R8G8B8].platformFormat = DXGI_FORMAT_R8G8B8A8_UINT;
	GTextureFormatInfo[R8G8B8A8_UINT].platformFormat = DXGI_FORMAT_R8G8B8A8_UINT;
	GTextureFormatInfo[R8G8B8A8_SNORM].platformFormat = DXGI_FORMAT_R8G8B8A8_SNORM;
	GTextureFormatInfo[DEPTH_STENCIL].platformFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	GTextureFormatInfo[SHADOW_DEPTH].platformFormat = DXGI_FORMAT_R16_TYPELESS;
}

D3D11RenderingInterface::~D3D11RenderingInterface() {
	if (d3dImmediateContext) {
		d3dImmediateContext->ClearState();
	}

	RELEASE(shadowState);
	RELEASE(defaultState);
	RELEASE(inputLayout);

	for (int i = 0; i < NumVertexConstantBuffers; i++) {
		RELEASE(vertexConstantBuffers[i]);
	}

	for (int i = 0; i < NumPixelConstantBuffers; i++) {
		RELEASE(pixelConstantBuffers[i]);
	}

	delete defaultDepthStencilBuffer;
	defaultDepthStencilBuffer = nullptr;

	delete defaultRenderTargetView;
	defaultRenderTargetView = nullptr;

	delete backBuffer;
	backBuffer = nullptr;

	delete backBufferDepthStencil;
	backBufferDepthStencil = nullptr;

	delete stateCache;
	stateCache = nullptr;
	RELEASE(d3dImmediateContext);
	RELEASE(swapChain);

#if defined(DEBUG) || defined(_DEBUG)  
	//ReportLiveObjects();
#endif
	RELEASE(d3dDevice);
}

void D3D11RenderingInterface::InitRenderer() {
	CreateDevice();
	CreateSwapChain();
	InitDefaultRenderTargets();
	CreateRasterStates();
}

bool D3D11RenderingInterface::CreateDevice() {
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	VERIFY_D3D_RESULT(
		D3D11CreateDevice(
			0, //default display adapter
			driverType,
			0, //not using software
			createDeviceFlags,
			0, //default feature level
			0, //no feature levels
			D3D11_SDK_VERSION,
			&d3dDevice,
			&featureLevel,
			&d3dImmediateContext
		);
	)

		if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
			return false;
		}

	stateCache = new RenderStateCache();
	stateCache->SetContext(d3dImmediateContext);

	return true;
}

bool D3D11RenderingInterface::CreateSwapChain() {
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (enable4xMsaa) {
		HRESULT hr(d3dDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			4,
			&xmsaaQuality
		));

		assert(xmsaaQuality > 0);

		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = xmsaaQuality - 1;
	}
	else {
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = windowHandle;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	VERIFY_D3D_RESULT(d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	VERIFY_D3D_RESULT(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	VERIFY_D3D_RESULT(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	VERIFY_D3D_RESULT(dxgiFactory->CreateSwapChain(d3dDevice, &sd, &swapChain));

	RELEASE(dxgiDevice);
	RELEASE(dxgiAdapter);
	RELEASE(dxgiFactory);

	return true;
}

void D3D11RenderingInterface::InitDefaultRenderTargets() {
	backBuffer = CreateBackBuffer();
	backBufferDepthStencil = CreateDepthAndStencilBuffer();
	defaultDepthStencilBuffer = new RenderTargetInfo(backBufferDepthStencil, 0);

	BindBackBuffer();
	SetViewPort(0.0f, 0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 1.0f);
}

D3D11Texture2d* D3D11RenderingInterface::CreateBackBuffer() {
	ID3D11Texture2D* backBufferResource;
	ID3D11RenderTargetView* renderTargetView;
	VERIFY_D3D_RESULT(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferResource)));
	VERIFY_D3D_RESULT(d3dDevice->CreateRenderTargetView(backBufferResource, 0, &renderTargetView));

	std::vector<ID3D11RenderTargetView*> rtvs;
	rtvs.push_back(renderTargetView);

	std::vector<ID3D11DepthStencilView*> dsvs;

	D3D11Texture2d* texture = new D3D11Texture2d(
		dsvs,
		nullptr,
		rtvs
	);

	RELEASE(backBufferResource);
	defaultRenderTargetView = new RenderTargetInfo(texture, 0);
	return texture;
}

D3D11Texture2d* D3D11RenderingInterface::CreateDepthAndStencilBuffer() {
	return CreateD3D11Texture2d(screenWidth, screenHeight, 1, false, false, 1, DEPTH_STENCIL, TextureBindAsDepthStencil, 4, nullptr);
}

VertexBuffer* D3D11RenderingInterface::CreateVertexBuffer(unsigned int size, const void * data) const {
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = size;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	ID3D11Buffer* bufferResource;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&vbd, &initData, &bufferResource));

	D3D11VertexBuffer* vertexBuffer = new D3D11VertexBuffer(bufferResource);
	return vertexBuffer;
}

IndexBuffer* D3D11RenderingInterface::CreateIndexBuffer(unsigned int size, const void * data) const {
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = size;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	ID3D11Buffer* bufferResource;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&ibd, &initData, &bufferResource));

	D3D11IndexBuffer* indexBuffer = new D3D11IndexBuffer(bufferResource);
	return indexBuffer;
}

ConstantBuffer* D3D11RenderingInterface::CreateConstantBuffer(unsigned int size) const {
	D3D11_BUFFER_DESC constantBufferDesc = {};

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = size;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	ID3D11Buffer* resource = nullptr;

	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &resource));

	D3D11ConstantBuffer* constantBuffer = new D3D11ConstantBuffer(resource);

	return constantBuffer;
}

VertexShader* D3D11RenderingInterface::CreateVertexShader(const unsigned char* shaderSource, size_t size) const {
	D3D11VertexShader* shader = new D3D11VertexShader();
	VERIFY_D3D_RESULT(d3dDevice->CreateVertexShader(shaderSource, size, nullptr, &shader->resource));

	return shader;
}

PixelShader* D3D11RenderingInterface::CreatePixelShader(const unsigned char* shaderSource, size_t size) const {
	D3D11PixelShader* shader = new D3D11PixelShader();
	VERIFY_D3D_RESULT(d3dDevice->CreatePixelShader(shaderSource, size, nullptr, &shader->resource));

	return shader;
}

GeometryShader * D3D11RenderingInterface::CreateGeometryShader(const unsigned char * shaderSource, size_t size) const {
	ID3D11GeometryShader* resource = nullptr;
	VERIFY_D3D_RESULT(d3dDevice->CreateGeometryShader(shaderSource, size, nullptr, &resource));

	D3D11GeometryShader* shader = new D3D11GeometryShader(resource);
	
	return shader;
}

SamplerState* D3D11RenderingInterface::CreateSamplerState(const SamplerConfig & config) const {
	D3D11_SAMPLER_DESC samplerDesc = {};

	switch (config.filter) {
	case ANISOTROPIC_FILTERING:
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	case POINT_FILTERING:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case BILINEAR_FILTERING:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	case TRILINEAR_FILTERING:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case COMPARE_BILINEAR_FILTERING:
		samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		break;	
	default:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	}

	samplerDesc.AddressU = GetAddressMode(config.addressModeU);
	samplerDesc.AddressV = GetAddressMode(config.addressModeV);
	samplerDesc.AddressW = GetAddressMode(config.addressModeW);
	samplerDesc.MipLODBias = config.mipLODBias ? config.mipLODBias : 0.0f;
	samplerDesc.ComparisonFunc = GetSamplerCompareFunction(config.comparisonFunction);
	samplerDesc.BorderColor[0] = config.borderColor[0];
	samplerDesc.BorderColor[1] = config.borderColor[1];
	samplerDesc.BorderColor[2] = config.borderColor[2];
	samplerDesc.BorderColor[3] = config.borderColor[3];

	samplerDesc.MinLOD = config.minLOD;
	samplerDesc.MaxLOD = config.maxLOD;

	ID3D11SamplerState* samplerState = nullptr;
	VERIFY_D3D_RESULT(d3dDevice->CreateSamplerState(&samplerDesc, &samplerState));

	D3D11SamplerState* d3d11SamplerState = new D3D11SamplerState();
	d3d11SamplerState->samplerState = samplerState;

	return d3d11SamplerState;
}

Texture2DRI * D3D11RenderingInterface::CreateTexture2d(unsigned int width, unsigned int height, unsigned int arraySize, bool isCube, bool isTextureArray, unsigned int numberOfMips, unsigned char format, unsigned int flags, unsigned int samples, const void* resourceDat) const {
	return CreateD3D11Texture2d(width, height, arraySize, isCube, isTextureArray, numberOfMips, format, flags, samples, resourceDat);
}

void D3D11RenderingInterface::UpdateConstantBuffer(ConstantBuffer* buffer, void * data, unsigned int size) const {
	D3D11_MAPPED_SUBRESOURCE subResources;
	VERIFY_D3D_RESULT(d3dImmediateContext->Map(static_cast<D3D11ConstantBuffer*>(buffer)->resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResources));
	memcpy(subResources.pData, data, size);
	d3dImmediateContext->Unmap(static_cast<D3D11ConstantBuffer*>(buffer)->resource, 0);
}

void D3D11RenderingInterface::SetVSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const {
	stateCache->SetConstantBuffer<VERTEX_SHADER>(static_cast<D3D11ConstantBuffer*>(buffer)->resource, slot);
}

void D3D11RenderingInterface::SetGSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const {
	stateCache->SetConstantBuffer<GEOMETRY_SHADER>(static_cast<D3D11ConstantBuffer*>(buffer)->resource, slot);
}

void D3D11RenderingInterface::SetPSConstantBuffer(ConstantBuffer* buffer, unsigned int slot) const {
	stateCache->SetConstantBuffer<PIXEL_SHADER>(static_cast<D3D11ConstantBuffer*>(buffer)->resource, slot);
}

void D3D11RenderingInterface::SetViewPort(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
	D3D11_VIEWPORT vp;
	vp.TopLeftX = minX;
	vp.TopLeftY = minY;
	vp.MinDepth = minZ;
	vp.Width = maxX;
	vp.Height = maxY;
	vp.MaxDepth = maxZ;

	stateCache->SetViewPort(vp);
}

void D3D11RenderingInterface::SetRenderTarget(unsigned int numberOfRenderTargets, RenderTargetInfo* renderTarget, RenderTargetInfo* depthTarget) {
	ID3D11DepthStencilView* depthStencilView = nullptr;

	ID3D11RenderTargetView* renderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

	for (unsigned int i = 0; i < numberOfRenderTargets; i++) {
		D3D11Texture* texture = dynamic_cast<D3D11Texture*>(renderTarget->texture);

		ID3D11RenderTargetView* newRenderTarget = texture->GetRenderTargetViews(renderTarget->arraySlice);
		renderTargets[i] = newRenderTarget;

		if (activeRenderTargetView[i] != newRenderTarget) {
			activeRenderTargetView[i] = renderTargets[i];
		}	
	}

	if (depthTarget) {
		D3D11Texture* texture = dynamic_cast<D3D11Texture*>(depthTarget->texture);
		depthStencilView = texture->GetDepthStencilView(depthTarget->arraySlice);
		activeDepthStencilBuffer = depthStencilView;
	}
	
	d3dImmediateContext->OMSetRenderTargets(numberOfRenderTargets, renderTargets, depthStencilView);
}

void D3D11RenderingInterface::BindBackBuffer() {
	SetRenderTarget(1, defaultRenderTargetView, defaultDepthStencilBuffer);
}

void D3D11RenderingInterface::SetVertexShader(VertexShader * shader) const {
	D3D11VertexShader* vShader = static_cast<D3D11VertexShader*>(shader);
	stateCache->SetVertexShader(vShader->resource);
}

void D3D11RenderingInterface::SetGeometryShader(GeometryShader * shader) const {
	D3D11GeometryShader* gShader = static_cast<D3D11GeometryShader*>(shader);

	if (gShader) {
		stateCache->SetGeometryShader(gShader->resource);
	} else {
		stateCache->SetGeometryShader(nullptr);
	}
}

void D3D11RenderingInterface::SetPixelShader(PixelShader * shader) const {
	D3D11PixelShader* pShader = static_cast<D3D11PixelShader*>(shader);
	if (pShader) {
		stateCache->SetPixelShader(pShader->resource);
	} else {
		stateCache->SetPixelShader(nullptr);
	}
}

D3D11Texture2d * D3D11RenderingInterface::CreateD3D11Texture2d(unsigned int width, unsigned int height, unsigned int arraySize, bool isCube, bool isTextureArray, unsigned int numberOfMips, unsigned char format, unsigned int flags, unsigned int samples, const void* resourceData) const {
	unsigned int actualXmsaaQuality;

	if (samples > 1) {
		actualXmsaaQuality = xmsaaQuality - 1;
	} else {
		actualXmsaaQuality = 0;
	}

	const DXGI_FORMAT textureFormat = static_cast<DXGI_FORMAT>(GTextureFormatInfo[format].platformFormat);

	unsigned int bindFlags = 0;
	bool shouldCreateDepthStencilView = false;
	if (flags & TextureBindAsDepthStencil) {
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;
		shouldCreateDepthStencilView = true;
	}

	bool shouldCreateShaderResourceView = false;
	if (flags & TextureBindAsShaderResource) {
		bindFlags |= D3D11_BIND_SHADER_RESOURCE;
		shouldCreateShaderResourceView = true;
	}

	bool shouldCreateRenderTargetView = false;
	if (flags & TextureBindAsRenderTarget) {
		bindFlags |= D3D11_BIND_RENDER_TARGET;
		shouldCreateRenderTargetView = true;
	}

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = numberOfMips;
	textureDesc.ArraySize = arraySize;
	textureDesc.Format = textureFormat;
	textureDesc.SampleDesc.Count = samples;
	textureDesc.SampleDesc.Quality = actualXmsaaQuality;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = isCube ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

	ID3D11Texture2D* texture;
	D3D11_SUBRESOURCE_DATA textureResourceData = {};
	if (resourceData) {
		textureResourceData.pSysMem = resourceData;

		//hardcoded value only for test
		textureResourceData.SysMemPitch = 1024 * 3;
	}

	VERIFY_D3D_RESULT(d3dDevice->CreateTexture2D(&textureDesc, resourceData ? &textureResourceData : 0, &texture));

	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	std::vector<ID3D11DepthStencilView*> depthStencileViews;
	std::vector<ID3D11RenderTargetView*> renderTargetViews;

	if (shouldCreateShaderResourceView) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = GetShaderResourceFormat(textureFormat);

		if (isCube && isTextureArray) {
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			srvDesc.TextureCubeArray.MipLevels = textureDesc.MipLevels;
			srvDesc.TextureCubeArray.MostDetailedMip = 0;
			srvDesc.TextureCubeArray.First2DArrayFace = 0;
			srvDesc.TextureCubeArray.NumCubes = textureDesc.ArraySize / 6;
		} else if (isCube) {
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = textureDesc.MipLevels;
			srvDesc.TextureCube.MostDetailedMip = 0;
		} else if (isTextureArray) {
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = 0;
			srvDesc.Texture2DArray.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2DArray.FirstArraySlice = 0;
			srvDesc.Texture2DArray.ArraySize = textureDesc.ArraySize;
		} else {
			srvDesc.ViewDimension = samples > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
		}

		VERIFY_D3D_RESULT(d3dDevice->CreateShaderResourceView(texture, &srvDesc, &shaderResourceView));
	}
	
	if (shouldCreateDepthStencilView) {
		if (flags & CreateRTVArraySlicesIndividualy && (isTextureArray || isCube)) {
			for (unsigned int arraySlice = 0; arraySlice < textureDesc.ArraySize; arraySlice++) {
				D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
				dsvDesc.Format = GetDepthStencilFormat(textureFormat);
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				dsvDesc.Texture2DArray.FirstArraySlice = arraySlice;
				dsvDesc.Texture2DArray.ArraySize = 1;

				ID3D11DepthStencilView* depthStencileView = nullptr;
				VERIFY_D3D_RESULT(d3dDevice->CreateDepthStencilView(texture, &dsvDesc, &depthStencileView));
				depthStencileViews.push_back(depthStencileView);
			}
		} else if (isCube || isTextureArray) {
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = GetDepthStencilFormat(textureFormat);
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			dsvDesc.Texture2DArray.FirstArraySlice = 0;
			dsvDesc.Texture2DArray.ArraySize = textureDesc.ArraySize;
			dsvDesc.Texture2DArray.MipSlice = 0;

			ID3D11DepthStencilView* depthStencileView = nullptr;
			VERIFY_D3D_RESULT(d3dDevice->CreateDepthStencilView(texture, &dsvDesc, &depthStencileView));
			depthStencileViews.push_back(depthStencileView);
		} else {
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = GetDepthStencilFormat(textureFormat);
			dsvDesc.ViewDimension = samples > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;

			ID3D11DepthStencilView* depthStencileView = nullptr;
			VERIFY_D3D_RESULT(d3dDevice->CreateDepthStencilView(texture, &dsvDesc, &depthStencileView));
			depthStencileViews.push_back(depthStencileView);
		}
	}

	if (shouldCreateRenderTargetView) {
		if (flags & CreateRTVArraySlicesIndividualy && (isTextureArray || isCube)) {
			for (unsigned int arraySlice = 0; arraySlice < textureDesc.ArraySize; arraySlice++) {
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
				rtvDesc.Format = GetShaderResourceFormat(textureFormat);
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				rtvDesc.Texture2DArray.FirstArraySlice = arraySlice;
				rtvDesc.Texture2DArray.ArraySize = 1;

				ID3D11RenderTargetView* renderTargetView = nullptr;
				VERIFY_D3D_RESULT(d3dDevice->CreateRenderTargetView(texture, &rtvDesc, &renderTargetView));
				renderTargetViews.push_back(renderTargetView);
			}
		} else if (isTextureArray || isCube) {
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = GetShaderResourceFormat(textureFormat);
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
			rtvDesc.Texture2DArray.ArraySize = textureDesc.ArraySize;

			ID3D11RenderTargetView* renderTargetView = nullptr;
			VERIFY_D3D_RESULT(d3dDevice->CreateRenderTargetView(texture, &rtvDesc, &renderTargetView));
			renderTargetViews.push_back(renderTargetView);
		} else {
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = GetShaderResourceFormat(textureFormat);
			rtvDesc.ViewDimension = samples > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
			ID3D11RenderTargetView* renderTargetView = nullptr;
			VERIFY_D3D_RESULT(d3dDevice->CreateRenderTargetView(texture, &rtvDesc, &renderTargetView));
			renderTargetViews.push_back(renderTargetView);
		}
	} else {
		renderTargetViews.push_back(nullptr);
	}

	RELEASE(texture);
	return new D3D11Texture2d(depthStencileViews, shaderResourceView, renderTargetViews);
}

D3D11_TEXTURE_ADDRESS_MODE D3D11RenderingInterface::GetAddressMode(AddressModes mode) const {
	switch (mode) {
		case WRAP:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		case CLAMP:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case BORDER:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		case MIRROR:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		default:
			return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}

D3D11_COMPARISON_FUNC D3D11RenderingInterface::GetSamplerCompareFunction(SamplerCompareFunction compare) const {
	switch (compare) {
		case NEVER:
			return D3D11_COMPARISON_NEVER;
		case LESS:
			return D3D11_COMPARISON_LESS;
		case LESS_OR_EQUAL:
			return D3D11_COMPARISON_LESS_EQUAL;
		case EQUAL:
			return D3D11_COMPARISON_EQUAL;
		case NOT_EQUAL:
			return D3D11_COMPARISON_NOT_EQUAL;
		case GREAT_OR_EQUAL:
			return D3D11_COMPARISON_GREATER_EQUAL;
		case GREATER:
			return D3D11_COMPARISON_GREATER;
		case ALWAYS:
			return D3D11_COMPARISON_ALWAYS;
		default:
			return D3D11_COMPARISON_NEVER;
	}
}

DXGI_FORMAT D3D11RenderingInterface::GetDepthStencilFormat(DXGI_FORMAT inFormat) const {
	switch (inFormat) {
		case DXGI_FORMAT_R16_TYPELESS :
			return DXGI_FORMAT_D16_UNORM;
		default :
			return inFormat;
		}
}

DXGI_FORMAT D3D11RenderingInterface::GetShaderResourceFormat(DXGI_FORMAT inFormat) const {
	switch (inFormat) {
	case DXGI_FORMAT_R16_TYPELESS:
		return DXGI_FORMAT_R16_UNORM;
	default:
		return inFormat;
	}
}

void D3D11RenderingInterface::CreateRasterStates() {
	D3D11_RASTERIZER_DESC shadowRasterStateDesc = {};
	shadowRasterStateDesc.FillMode = D3D11_FILL_SOLID;
	shadowRasterStateDesc.CullMode = D3D11_CULL_BACK;
	shadowRasterStateDesc.DepthClipEnable = true;
	shadowRasterStateDesc.DepthBias = 50;
	shadowRasterStateDesc.DepthBiasClamp = 0.0f;
	shadowRasterStateDesc.SlopeScaledDepthBias = 1.0f;

	D3D11_RASTERIZER_DESC defaultStateDesc = {};
	defaultStateDesc.FillMode = D3D11_FILL_SOLID;
	defaultStateDesc.CullMode = D3D11_CULL_BACK;
	defaultStateDesc.DepthClipEnable = true;
	defaultStateDesc.DepthBias = 0;
	defaultStateDesc.DepthBiasClamp = 0.0f;
	defaultStateDesc.SlopeScaledDepthBias = 0.0f;

	VERIFY_D3D_RESULT(d3dDevice->CreateRasterizerState(&shadowRasterStateDesc, &shadowState));
	VERIFY_D3D_RESULT(d3dDevice->CreateRasterizerState(&defaultStateDesc, &defaultState));
}

void D3D11RenderingInterface::ReportLiveObjects() {
	ID3D11Debug* debugDevice = nullptr;
	HRESULT result = d3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));
	RELEASE(d3dDevice);
	result = debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	RELEASE(debugDevice);
}

void D3D11RenderingInterface::CreateInputLayout(const unsigned char* shaderSource, size_t size) {

	// hardcoded values for now
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	VERIFY_D3D_RESULT(d3dDevice->CreateInputLayout(vertexDesc, 3, shaderSource, size, &inputLayout));
}

void D3D11RenderingInterface::ConstantBuffersMiddFrame(ObjectProperties objectProperties, MaterialResource material) const {
	D3D11_MAPPED_SUBRESOURCE objecResources;
	VERIFY_D3D_RESULT(d3dImmediateContext->Map(vertexConstantBuffers[WorldViewPorjectionConstBuffer], 0, D3D11_MAP_WRITE_DISCARD, 0, &objecResources));
	memcpy(objecResources.pData, &objectProperties, sizeof(objectProperties));
	d3dImmediateContext->Unmap(vertexConstantBuffers[WorldViewPorjectionConstBuffer], 0);

	D3D11_MAPPED_SUBRESOURCE materialResources;
	VERIFY_D3D_RESULT(d3dImmediateContext->Map(pixelConstantBuffers[MaterialConstBuffer], 0, D3D11_MAP_WRITE_DISCARD, 0, &materialResources));
	memcpy(materialResources.pData, &material, sizeof(material));
	d3dImmediateContext->Unmap(pixelConstantBuffers[MaterialConstBuffer], 0);
}

void D3D11RenderingInterface::UpdateShadowConstantBuffer(VertexShaderShadowResources shadowResources) const {
	D3D11_MAPPED_SUBRESOURCE resources;
	VERIFY_D3D_RESULT(d3dImmediateContext->Map(vertexConstantBuffers[ShadowConstantBuffer], 0, D3D11_MAP_WRITE_DISCARD, 0, &resources));
	memcpy(resources.pData, &shadowResources, sizeof(shadowResources));
	d3dImmediateContext->Unmap(vertexConstantBuffers[ShadowConstantBuffer], 0);
}

void D3D11RenderingInterface::ConstantBuffersFrameStart(PixelShaderPerFrameResource perFrameResources) const {
	D3D11_MAPPED_SUBRESOURCE lightResources;
	VERIFY_D3D_RESULT(d3dImmediateContext->Map(pixelConstantBuffers[DirectionalLightsConstBuffer], 0, D3D11_MAP_WRITE_DISCARD, 0, &lightResources));
	memcpy(lightResources.pData, &perFrameResources, sizeof(perFrameResources));
	d3dImmediateContext->Unmap(pixelConstantBuffers[DirectionalLightsConstBuffer], 0);
}

void D3D11RenderingInterface::ClearActiveRenderTarget() const {
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	for (unsigned int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		if (activeRenderTargetView[i]) {
			d3dImmediateContext->ClearRenderTargetView(activeRenderTargetView[i], black);
		}
	}

	if (activeDepthStencilBuffer) {
		d3dImmediateContext->ClearDepthStencilView(activeDepthStencilBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void D3D11RenderingInterface::SetShaderResources(TextureRI * shaderResource, unsigned int slot) const {
	ID3D11ShaderResourceView * shaderResourceView = dynamic_cast<D3D11Texture*>(shaderResource)->GetShaderResourceView();
	d3dImmediateContext->PSSetShaderResources(slot, 1, &shaderResourceView);
}

void D3D11RenderingInterface::SetSamplerState(SamplerState * samplerState, unsigned int slot) const {
	ID3D11SamplerState* sampler = static_cast<D3D11SamplerState*>(samplerState)->samplerState;
	d3dImmediateContext->PSSetSamplers(slot, 1, &sampler);
}

void D3D11RenderingInterface::ClearShaderResource(unsigned int slot) const {
	ID3D11ShaderResourceView* null[] = { nullptr };
	d3dImmediateContext->PSSetShaderResources(slot, 1, null);
}

void D3D11RenderingInterface::SetShadowRasterState() const {
	d3dImmediateContext->RSSetState(shadowState);
}

void D3D11RenderingInterface::SetDeafultRasterState() const {
	d3dImmediateContext->RSSetState(defaultState);
}

void D3D11RenderingInterface::StartFrame() const {
	stateCache->SetInputLayout(inputLayout);
	stateCache->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D11RenderingInterface::Draw(RenderData* renderData, VertexShader* vertexShader, PixelShader* pixelShader) {
	//vertex buffer
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	D3D11VertexBuffer* vertexBuffer = static_cast<D3D11VertexBuffer*>(renderData->GetVertexBuffer());
	D3D11IndexBuffer* indexBuffer = static_cast<D3D11IndexBuffer*>(renderData->GetIndexBuffer());

	d3dImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer->resource, &stride, &offset);
	d3dImmediateContext->IASetIndexBuffer(indexBuffer->resource, DXGI_FORMAT_R32_UINT, 0);

	//vertex shader
	SetVertexShader(vertexShader);

	//rasterizer

	//pixel shader
	SetPixelShader(pixelShader);

	d3dImmediateContext->DrawIndexed(renderData->GetNumIndices(), 0, 0);
}

void D3D11RenderingInterface::EndFrame() const {
	swapChain->Present(0, 0);
}

