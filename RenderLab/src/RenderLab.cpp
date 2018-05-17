#include "RenderLab.h"
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
RenderLab::RenderLab(HWND window) :
	windowHandle(window),
	renderer (nullptr),
	camera (nullptr),
	mesh(nullptr),
	world(nullptr),
	light(nullptr),
	testRotation(0) {
};

RenderLab::~RenderLab() {
	delete camera;
	camera = nullptr;

	delete mesh;
	mesh = nullptr;

	delete vertexBuffer;
	vertexBuffer = nullptr;

	delete indexBuffer;
	indexBuffer = nullptr;

	delete vertextShader;
	vertextShader = nullptr;

	delete pixelShader;
	pixelShader = nullptr;

	delete renderer;
	renderer = nullptr;

	delete light;
	light = nullptr;

	delete world;
	world = nullptr;
};

bool RenderLab::InitLab() {
	if (!CreateRenderer()) {
		return false;
	}

	CreateWorld();
	CreateLights();
	BuildGeometry();
	InitShaders();
	CreateCamera();
	CreateMesh();

	return true;
}

void RenderLab::Tick() {
	timer.Tick();

	testRotation += 50 * timer.GetDeltaTime();

	mesh->SetRotation(testRotation, testRotation, 0);
}

bool RenderLab::CreateRenderer() {
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

	camera->SetPosition(0.0f, 5.0f, 10.0f);
	camera->SetCameraTarget(0, 2.5f, -10);

	camera->UpdateProjection();

	world->AddCamera(camera);
}

void RenderLab::CreateMesh() {
	mesh = new StaticMesh();
	mesh->SetVertexBuffer(vertexBuffer);
	mesh->SetIndexBuffer(indexBuffer);
	mesh->SetVertexShader(vertextShader);
	mesh->SetPixelShader(pixelShader);

	mesh->SetPosition(0, 0, -20);
	mesh->SetRotation(0, testRotation, 0);

	world->AddStaticMesh(mesh);
}

void RenderLab::CreateWorld() {
	world = new World();
}

void RenderLab::CreateLights() {
	light = new DirectionalLightComponent();
	light->SetPosition(-10, 20, -10);
	light->SetDirection(-.2, -.4, -1);
	light->SetLightColor(1.0f, 1.0f, 1.0f);
	light->SetIsEnabled(true);

	world->AddDirectionalLight(light);
}

void RenderLab::Draw() {
	mesh->UpdateTransform();
	camera->UpdateView();
	renderer->RenderWorld(world);
}

void RenderLab::PrepareStart() {
	timer.Reset();
}

void RenderLab::ShutDown() {

}
