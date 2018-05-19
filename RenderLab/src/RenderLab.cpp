#include "RenderLab.h"
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
RenderLab::RenderLab(HWND window) :
	windowHandle(window),
	renderer (nullptr),
	camera (nullptr),
	box(nullptr),
	box2(nullptr),
	sphere(nullptr),
	plane(nullptr),
	world(nullptr),
	light(nullptr),
	boxMaterial(nullptr),
	box2Material(nullptr),
	sphereMaterial(nullptr),
	planeMaterial(nullptr),
	testRotation(0) {
};

RenderLab::~RenderLab() {
	delete camera;
	camera = nullptr;

	delete box;
	box = nullptr;

	delete box2;
	box2 = nullptr;

	delete plane;
	plane = nullptr;

	delete sphere;
	sphere = nullptr;

	delete vertexBuffer;
	vertexBuffer = nullptr;

	delete indexBuffer;
	indexBuffer = nullptr;

	delete planeVertexBuffer;
	planeVertexBuffer = nullptr;

	delete planeIndexBuffer;
	planeIndexBuffer = nullptr;

	delete sphereVertexBuffer;
	sphereVertexBuffer = nullptr;

	delete sphereIndexBuffer;
	sphereIndexBuffer = nullptr;

	delete vertextShader;
	vertextShader = nullptr;

	delete pixelShader;
	pixelShader = nullptr;

	delete renderer;
	renderer = nullptr;

	delete boxMaterial;
	boxMaterial = nullptr;

	delete box2Material;
	box2Material = nullptr;

	delete sphereMaterial;
	sphereMaterial = nullptr;

	delete planeMaterial;
	planeMaterial = nullptr;

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
	InitShaders();
	BuildGeometry();
	CreateCamera();
	CreateMesh();

	return true;
}

void RenderLab::Tick() {
	timer.Tick();

	testRotation += 50 * timer.GetDeltaTime();

	//box->SetRotation(testRotation, testRotation, 0);
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

	MeshData boxData;

	factory->CreateBox(5, 5, 5, boxData);

	size_t vertSize = sizeof(Vertex) * boxData.vertices.size();
	vertexBuffer = renderer->CreateVertexBuffer(vertSize, &boxData.vertices[0]);

	size_t indSize = sizeof(unsigned int) * boxData.indices.size();
	indexBuffer = renderer->CreateIndexBuffer(indSize, &boxData.indices[0]);

	MeshData planeData;

	factory->CreatePlane(30, 30, 2, 2, planeData);

	size_t planeVertSize = sizeof(Vertex) * planeData.vertices.size();
	planeVertexBuffer = renderer->CreateVertexBuffer(planeVertSize, &planeData.vertices[0]);

	size_t planeIndSize = sizeof(unsigned int) * planeData.indices.size();
	planeIndexBuffer = renderer->CreateIndexBuffer(planeIndSize, &planeData.indices[0]);

	MeshData sphereData;

	factory->CreateSphere(3.0f, 40, 20, sphereData);

	size_t sphereVertSize = sizeof(Vertex) * sphereData.vertices.size();
	sphereVertexBuffer = renderer->CreateVertexBuffer(sphereVertSize, &sphereData.vertices[0]);

	size_t sphereIndSize = sizeof(unsigned int) * sphereData.indices.size();
	sphereIndexBuffer = renderer->CreateIndexBuffer(sphereIndSize, &sphereData.indices[0]);

	//  ---- box ----- //

	box = new StaticMesh();

	RenderData* boxRenderData = new RenderData();
	boxRenderData->SetVertexBuffer(vertexBuffer);
	boxRenderData->SetIndexBuffer(indexBuffer);
	boxRenderData->SetNumIndices(boxData.indices.size());

	box->SetVertexShader(vertextShader);
	box->SetPixelShader(pixelShader);
	box->SetRenderData(boxRenderData);

	box->SetPosition(-4, 2.5f, -10);
	box->SetRotation(0, 30, 0);

	boxMaterial = new Material();
	boxMaterial->SetAlbedo(0.87f, 0.53f, 0.9f);

	box->SetMaterial(boxMaterial);

	box2 = new StaticMesh();

	RenderData* box2RenderData = new RenderData();
	box2RenderData->SetVertexBuffer(vertexBuffer);
	box2RenderData->SetIndexBuffer(indexBuffer);
	box2RenderData->SetNumIndices(boxData.indices.size());

	box2->SetVertexShader(vertextShader);
	box2->SetPixelShader(pixelShader);
	box2->SetRenderData(box2RenderData);

	box2->SetPosition(4, 2.5f, 4);
	box2->SetRotation(0, 145, 0);

	box2Material = new Material();
	box2Material->SetAlbedo(0.533f, 0.72f, 0.9f);

	box2->SetMaterial(box2Material);

	//  ---- plane ----- //

	plane = new StaticMesh();

	RenderData* planeRenderData = new RenderData();
	planeRenderData->SetVertexBuffer(planeVertexBuffer);
	planeRenderData->SetIndexBuffer(planeIndexBuffer);
	planeRenderData->SetNumIndices(planeData.indices.size());

	plane->SetVertexShader(vertextShader);
	plane->SetPixelShader(pixelShader);
	plane->SetRenderData(planeRenderData);

	plane->SetPosition(0, 0, 0);

	planeMaterial = new Material();
	planeMaterial->SetAlbedo(0.92f, 0.7f, 0.35f);
	plane->SetMaterial(planeMaterial);

	//  ---- sphere ----- //

	sphere = new StaticMesh();

	RenderData* sphereRenderData = new RenderData();
	sphereRenderData->SetVertexBuffer(sphereVertexBuffer);
	sphereRenderData->SetIndexBuffer(sphereIndexBuffer);
	sphereRenderData->SetNumIndices(sphereData.indices.size());

	sphere->SetVertexShader(vertextShader);
	sphere->SetPixelShader(pixelShader);
	sphere->SetRenderData(sphereRenderData);

	sphere->SetPosition(-4.0, 3.0f, 3.0f);

	sphereMaterial = new Material();
	sphereMaterial->SetAlbedo(0.74f, 0.9f, 0.533f);
	sphere->SetMaterial(sphereMaterial);

	world->AddStaticMesh(box);
	world->AddStaticMesh(box2);
	world->AddStaticMesh(plane);
	world->AddStaticMesh(sphere);

	renderer->CreateConstantBuffer();

	delete factory;
}

void RenderLab::CreateCamera() {
	camera = new CameraComponent();
	camera->SetFieldOfView(3.14f / 4.0f);
	camera->SetAspectratio(1280.0f / 720.0f);
	camera->SetNearPlane(1);
	camera->SetFarPlane(1000);

	camera->SetPosition(0.0f, 8.0f, 25.0f);
	camera->SetCameraTarget(0, 2.5f, -10);

	camera->UpdateProjection();

	world->AddCamera(camera);
}

void RenderLab::CreateMesh() {

}

void RenderLab::CreateWorld() {
	world = new World();
}

void RenderLab::CreateLights() {
	light = new DirectionalLightComponent();
	light->SetPosition(-10, 20, -10);
	light->SetDirection(-0.3f, -.6f, -1.0f);
	light->SetLightColor(1.0f, 1.0f, 1.0f);
	light->SetBrightness(2.6f);
	light->SetIsEnabled(true);

	world->AddDirectionalLight(light);
}

void RenderLab::Draw() {
	box->UpdateTransform();
	box2->UpdateTransform();
	plane->UpdateTransform();
	sphere->UpdateTransform();
	camera->UpdateView();
	renderer->RenderWorld(world);
}

void RenderLab::PrepareStart() {
	timer.Reset();
}

void RenderLab::ShutDown() {

}
