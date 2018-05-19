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
	depthStencilView(nullptr),
	windowHandle(window),
	enable4xMsaa(true),
	xmsaaQuality(0),
	driverType(D3D_DRIVER_TYPE_HARDWARE)
{}

D3D11RenderingInterface::~D3D11RenderingInterface() {
	if (d3dImmediateContext) {
		d3dImmediateContext->ClearState();
	}

	RELEASE(inputLayout);

	for (int i = 0; i < NumConstantBuffers; i++) {
		RELEASE(constantBuffers[i]);
	}

	RELEASE(depthStencilView);
	RELEASE(depthStencilBuffer);
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
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = screenWidth;
	depthStencilDesc.Height = screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (enable4xMsaa) {
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = xmsaaQuality - 1;
	}
	else {
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	depthStencilBuffer = CreateTexture2D(&depthStencilDesc);
	depthStencilView;

	VERIFY_D3D_RESULT(d3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView));

	d3dImmediateContext->OMSetRenderTargets(1, &d3dRenderTargetView, depthStencilView);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(screenWidth);
	vp.Height = static_cast<float>(screenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	d3dImmediateContext->RSSetViewports(1, &vp);
}

ID3D11Texture2D* D3D11RenderingInterface::CreateTexture2D(D3D11_TEXTURE2D_DESC* desc) const {
	ID3D11Texture2D* texture;
	VERIFY_D3D_RESULT(d3dDevice->CreateTexture2D(desc, 0, &texture));
	return texture;
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

	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&wvpConstantBufferDesc, nullptr, &constantBuffers[WorldViewPorjection]));

	D3D11_BUFFER_DESC dlConstantBufferDesc;
	ZeroMemory(&dlConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	dlConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dlConstantBufferDesc.ByteWidth = sizeof(DirectionalLightResource);
	dlConstantBufferDesc.CPUAccessFlags = 0;
	dlConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VERIFY_D3D_RESULT(d3dDevice->CreateBuffer(&dlConstantBufferDesc, nullptr, &constantBuffers[DirectionalLights]));
}

void D3D11RenderingInterface::ConstantBuffersMiddFrame(ObjectProperties objectProperties) const {
	d3dImmediateContext->UpdateSubresource(constantBuffers[WorldViewPorjection], 0, nullptr, &objectProperties, 0, 0);
}

void D3D11RenderingInterface::ConstantBuffersFrameStart(DirectionalLightResource light) const {
	d3dImmediateContext->UpdateSubresource(constantBuffers[DirectionalLights], 0, nullptr, &light, 0, 0);
}

void D3D11RenderingInterface::StartFrame() const {
	//clear
	float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	d3dImmediateContext->ClearRenderTargetView(d3dRenderTargetView, black);
	d3dImmediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	d3dImmediateContext->IASetInputLayout(inputLayout);
	d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

	d3dImmediateContext->VSSetShader(vShader->resource, nullptr, 0);
	d3dImmediateContext->VSSetConstantBuffers(0, 1, &constantBuffers[WorldViewPorjection]);

	//rasterizer

	//pixel shader
	D3D11PixelShader* pShader = static_cast<D3D11PixelShader*>(pixelShader);
	d3dImmediateContext->PSSetShader(pShader->resource, nullptr, 0);
	d3dImmediateContext->PSSetConstantBuffers(0, 1, &constantBuffers[DirectionalLights]);

	d3dImmediateContext->DrawIndexed(renderData->GetNumIndices(), 0, 0);
}

void D3D11RenderingInterface::EndFrame() const {
	swapChain->Present(0, 0);
}

