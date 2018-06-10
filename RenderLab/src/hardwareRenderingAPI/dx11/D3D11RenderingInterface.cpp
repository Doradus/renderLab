#include "D3D11RenderingInterface.h"
#include "MathUtils.h"
#include <cassert>

D3D11RenderingInterface::D3D11RenderingInterface(int screenWidth, int screenHeight, HWND window) :
	screenWidth(screenWidth),
	screenHeight(screenHeight),
	d3dDevice(nullptr),
	d3dImmediateContext(nullptr),
	d3dRenderTargetView(nullptr),
	swapChain(nullptr),
	depthStencilBuffer(nullptr),
	windowHandle(window),
	enable4xMsaa(true),
	xmsaaQuality(0),
	stateCache(nullptr),
	driverType(D3D_DRIVER_TYPE_HARDWARE)
{
	GTextureFormatInfo[UNKNOWN].platformFormat = DXGI_FORMAT_UNKNOWN;
	GTextureFormatInfo[R8G8B8A8_UINT].platformFormat = DXGI_FORMAT_R8G8B8A8_UINT;
	GTextureFormatInfo[R8G8B8A8_SNORM].platformFormat = DXGI_FORMAT_R8G8B8A8_SNORM;
	GTextureFormatInfo[DEPTH_STENCIL].platformFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	GTextureFormatInfo[SHADOW_DEPTH].platformFormat = DXGI_FORMAT_R16_TYPELESS;
}

D3D11RenderingInterface::~D3D11RenderingInterface() {
	if (d3dImmediateContext) {
		d3dImmediateContext->ClearState();
	}

	RELEASE(inputLayout);

	for (int i = 0; i < NumVertexConstantBuffers; i++) {
		RELEASE(vertexConstantBuffers[i]);
	}

	for (int i = 0; i < NumPixelConstantBuffers; i++) {
		RELEASE(pixelConstantBuffers[i]);
	}

	delete depthStencilBuffer;
	depthStencilBuffer = nullptr;
	delete stateCache;
	stateCache = nullptr;
	RELEASE(d3dRenderTargetView);
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
	CreateRenderTarget();
	CreateDepthAndStencilBuffer();
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

void D3D11RenderingInterface::CreateRenderTarget() {
	ID3D11Texture2D* backBuffer;

	VERIFY_D3D_RESULT(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	VERIFY_D3D_RESULT(d3dDevice->CreateRenderTargetView(backBuffer, 0, &d3dRenderTargetView));

	RELEASE(backBuffer);
}

void D3D11RenderingInterface::CreateDepthAndStencilBuffer() {
	depthStencilBuffer = CreateTexture2d(screenWidth, screenHeight, 1, DEPTH_STENCIL, TextureBindAsDepthStencil);
	d3dImmediateContext->OMSetRenderTargets(1, &d3dRenderTargetView, depthStencilBuffer->GetDepthStencilView());

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(screenWidth);
	vp.Height = static_cast<float>(screenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	d3dImmediateContext->RSSetViewports(1, &vp);
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
	default:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	}

	samplerDesc.AddressU = GetAddressMode(config.addressModeU);
	samplerDesc.AddressV = GetAddressMode(config.addressModeV);
	samplerDesc.AddressW = GetAddressMode(config.addressModeW);
	samplerDesc.MipLODBias = config.mipLODBias;
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

D3D11Texture2d * D3D11RenderingInterface::CreateTexture2d(unsigned int width, unsigned int height, unsigned int numberOfMips, unsigned char format, unsigned int flags) const {
	unsigned int numSamples;
	unsigned int actualXmsaaQuality;

	if (enable4xMsaa) {
		numSamples = 4;
		actualXmsaaQuality = xmsaaQuality - 1;
	} else {
		numSamples = 1;
		actualXmsaaQuality = 0;
	}

	const DXGI_FORMAT textureFormat = static_cast<DXGI_FORMAT>(GTextureFormatInfo[format].platformFormat);

	unsigned int bindFlags = 0;
	bool shouldCreateDepthStencilView = false;
	if (flags & TextureBindAsDepthStencil) {
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;
		shouldCreateDepthStencilView = true;
	}

	bool shouldShaderResourceView = false;
	if (flags & TextureBindAsShaderResource) {
		bindFlags |= D3D11_BIND_DEPTH_STENCIL;
		shouldShaderResourceView = true;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = numberOfMips;
	textureDesc.ArraySize = 1;
	textureDesc.Format = textureFormat;
	textureDesc.SampleDesc.Count = numSamples;
	textureDesc.SampleDesc.Quality = actualXmsaaQuality;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* texture;
	VERIFY_D3D_RESULT(d3dDevice->CreateTexture2D(&textureDesc, 0, &texture));

	ID3D11DepthStencilView* depthStencileView = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	ID3D11RenderTargetView*	renderTargetView = nullptr;

	if (shouldShaderResourceView) {
		VERIFY_D3D_RESULT(d3dDevice->CreateShaderResourceView(texture, 0, &shaderResourceView));
	}
	
	if (shouldCreateDepthStencilView) {
		VERIFY_D3D_RESULT(d3dDevice->CreateDepthStencilView(texture, 0, &depthStencileView));
	}

	RELEASE(texture);
	return new D3D11Texture2d(depthStencileView, shaderResourceView, renderTargetView);
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

void D3D11RenderingInterface::CreateInputLayout(const unsigned char* shaderSource, size_t size) {

	// hardcoded values for now
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	VERIFY_D3D_RESULT(d3dDevice->CreateInputLayout(vertexDesc, 3, shaderSource, size, &inputLayout));
}

void D3D11RenderingInterface::CreateConstantBuffer() {
	D3D11_BUFFER_DESC wvpConstantBufferDesc;
	ZeroMemory(&wvpConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	wvpConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	wvpConstantBufferDesc.ByteWidth = sizeof(ObjectProperties);
	wvpConstantBufferDesc.CPUAccessFlags = 0;
	wvpConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&wvpConstantBufferDesc, nullptr, &vertexConstantBuffers[WorldViewPorjectionConstBuffer]));

	D3D11_BUFFER_DESC dlConstantBufferDesc;
	ZeroMemory(&dlConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	dlConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dlConstantBufferDesc.ByteWidth = sizeof(PixelShaderPerFrameResource);
	dlConstantBufferDesc.CPUAccessFlags = 0;
	dlConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&dlConstantBufferDesc, nullptr, &pixelConstantBuffers[DirectionalLightsConstBuffer]));

	D3D11_BUFFER_DESC materialConstantBufferDesc;
	ZeroMemory(&materialConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	materialConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialConstantBufferDesc.ByteWidth = sizeof(MaterialResource);
	materialConstantBufferDesc.CPUAccessFlags = 0;
	materialConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&materialConstantBufferDesc, nullptr, &pixelConstantBuffers[MaterialConstBuffer]));
}

void D3D11RenderingInterface::ConstantBuffersMiddFrame(ObjectProperties objectProperties, MaterialResource material) const {
	d3dImmediateContext->UpdateSubresource(vertexConstantBuffers[WorldViewPorjectionConstBuffer], 0, nullptr, &objectProperties, 0, 0);
	d3dImmediateContext->UpdateSubresource(pixelConstantBuffers[MaterialConstBuffer], 0, nullptr, &material, 0, 0);
}

void D3D11RenderingInterface::ConstantBuffersFrameStart(PixelShaderPerFrameResource perFrameResources) const {
	d3dImmediateContext->UpdateSubresource(pixelConstantBuffers[DirectionalLightsConstBuffer], 0, nullptr, &perFrameResources, 0, 0);
}

void D3D11RenderingInterface::StartFrame() const {
	//clear
	float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	d3dImmediateContext->ClearRenderTargetView(d3dRenderTargetView, black);
	d3dImmediateContext->ClearDepthStencilView(depthStencilBuffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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
	D3D11VertexShader* vShader = static_cast<D3D11VertexShader*>(vertexShader);

	stateCache->SetVertexShader(vShader->resource);
	d3dImmediateContext->VSSetConstantBuffers(0, 1, &vertexConstantBuffers[WorldViewPorjectionConstBuffer]);

	//rasterizer

	//pixel shader
	D3D11PixelShader* pShader = static_cast<D3D11PixelShader*>(pixelShader);
	stateCache->SetPixelShader(pShader->resource);
	d3dImmediateContext->PSSetConstantBuffers(0, 2, pixelConstantBuffers);

	d3dImmediateContext->DrawIndexed(renderData->GetNumIndices(), 0, 0);
}

void D3D11RenderingInterface::EndFrame() const {
	swapChain->Present(0, 0);
}

