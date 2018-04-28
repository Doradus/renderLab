#include "RenderLab.h"
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
RenderLab::RenderLab(HWND window) :
	windowHandle(window),
	renderer (nullptr),
	camera (nullptr),
	mesh(nullptr) {
};

RenderLab::~RenderLab() {
	delete camera;
	camera = nullptr;

	delete mesh;
	mesh = nullptr;

	delete renderer;
	renderer = nullptr;

	delete vertexBuffer;
	vertexBuffer = nullptr;
	delete indexBuffer;
	indexBuffer = nullptr;

	delete vertextShader;
	delete pixelShader;

};

bool RenderLab::InitLab() {
	if (!CreateRenderingInterface()) {
		return false;
	}

	BuildGeometry();
	InitShaders();
	CreateCamera();
	CreateMesh();
	Draw();

	return true;
};

bool RenderLab::CreateRenderingInterface() {
	renderer = new Renderer();
	renderer->CreateHardwareRenderingInterface(1280, 720, windowHandle);
	return true;
}

void RenderLab::InitShaders() {
	size_t size = sizeof(g_basic_vs);
	vertextShader  = renderer->CreateVertexShader(g_basic_vs, size);

	size_t pixelShaderSize = sizeof(g_ps);
	pixelShader = renderer->CreatePixelShader(g_ps, pixelShaderSize);

	renderer->CreateInputLayout(g_basic_vs, size);
}

void RenderLab::BuildGeometry() {
	PrimitiveFactory* factory = new PrimitiveFactory();

	MeshData box;

	factory->CreateBox(5, 5, 5, box);

	size_t vertSize = sizeof(Vertex) * box.vertices.size();
	vertexBuffer = renderer->CreateVertexBuffer(vertSize, &box.vertices[0]);

	size_t indSize = sizeof(unsigned int) * box.indices.size();
	indexBuffer = renderer->CreateIndexBuffer(indSize, &box.indices[0]);

	renderer->CreateConstantBuffer();

	delete factory;
}

void RenderLab::CreateCamera() {
	camera = new CameraComponent();
	camera->SetFieldOfView(3.14f / 4.0f);
	camera->SetAspectratio(1280.0f / 720.0f);
	camera->SetNearPlane(1);
	camera->SetFarPlane(1000);

	camera->SetPosition(0.0f, 5.0f, 0.0f);
	camera->SetCameraTarget(0, 0, -10);

	camera->UpdateProjection();
}

void RenderLab::CreateMesh() {
	mesh = new StaticMesh();
	mesh->SetVertexBuffer(vertexBuffer);
	mesh->SetIndexBuffer(indexBuffer);
	mesh->SetVertexShader(vertextShader);
	mesh->SetPixelShader(pixelShader);

	mesh->SetPosition(0, 0, -20);
	mesh->SetRotation(0, 30, 0);
}

void RenderLab::Draw() {
	mesh->UpdateTransform();
	camera->UpdateView();

	//update const buffer
	XMMATRIX world = XMLoadFloat4x4(&mesh->GetWorld());
	XMMATRIX view = XMLoadFloat4x4(&camera->GetCameraView());
	XMMATRIX proj = XMLoadFloat4x4(&camera->GetProjection());

	XMMATRIX wvp = world * view * proj;
	XMMATRIX transposedWvp = XMMatrixTranspose(wvp);

	XMFLOAT4X4 wvpData;


	XMStoreFloat4x4(&wvpData, transposedWvp);

	renderer->UpdateConstantBuffer(wvpData);
	renderer->RenderPrimitive(mesh);
}

void RenderLab::ShutDown() {

}
