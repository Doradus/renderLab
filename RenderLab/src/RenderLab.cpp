#include "RenderLab.h"
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
	pointLight(nullptr),
	spotLight(nullptr),
	spotLight2(nullptr),
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

	//delete pixelShader;
	//pixelShader = nullptr;

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

	delete pointLight;
	pointLight = nullptr;

	delete spotLight;
	spotLight = nullptr;

	delete spotLight2;
	spotLight2 = nullptr;

	delete world;
	world = nullptr;

	ResourceManager::GetInstance().DestroyAllTextures();
};

bool RenderLab::InitLab() {
	if (!CreateRenderer()) {
		return false;
	}

	CreateResources();
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

	testRotation += timer.GetDeltaTime();
	const float x = 15 * sinf(testRotation);

	//ight->SetDirection(-0.4f, -.5f, testRotation);
	pointLight->SetPosition(x, 8, -5);
	//box2->SetRotation(0, testRotation, 0);
}

void RenderLab::CreateResources() const {
	ResourceManager::GetInstance().CreateTextureFromFile("OutputCube.dds", true);
	ResourceManager::GetInstance().CreateTextureFromFile("TexturesCom_Marble_SlabRed_1K_albedo.dds", true);
	ResourceManager::GetInstance().CreateTextureFromFile("floor_NRM.dds", true);
	ResourceManager::GetInstance().CreateTextureFromFile("TexturesCom_Marble_SlabRed_1K_roughness.dds", true);
}

bool RenderLab::CreateRenderer() {
	renderer = new Renderer();
	renderer->CreateHardwareRenderingInterface(1920, 1080, windowHandle);
	return true;
}

void RenderLab::InitShaders() {
	char* byteCode = nullptr;
	unsigned int byteCodeSize = 0;
	 
	ResourceManager::GetInstance().GetShaderByteCode("shaders/BasicVertexShader.hlsl", VERTEX_SHADER, nullptr, 0, &byteCodeSize, &byteCode);

	char* vertexShaderCode = byteCode;
	vertextShader  = renderingInterface->CreateVertexShader(vertexShaderCode, byteCodeSize);

	const unsigned char* vertexCode = (unsigned char*)byteCode;
	renderer->CreateInputLayout(vertexCode, byteCodeSize);

	boxMaterial = new Material();
	boxMaterial->SetSpecularColor(0.2f, 0.2f, 0.2f);
	boxMaterial->SetSpecularPower(20.0f);

	TextureSamplerNode* albedo = new TextureSamplerNode(boxMaterial);
	albedo->AddTexture(ResourceManager::GetInstance().GetTexture("TexturesCom_Marble_SlabRed_1K_albedo.dds"));

	//Vector3Node* albedo = new Vector3Node(boxMaterial);
	//albedo->SetValues(0.245f, 0.245f, 0.245f);

	boxMaterial->AddMaterialNode(albedo);
	boxMaterial->SetAlbedo(albedo);

	//TextureSamplerNode* normal = new TextureSamplerNode(boxMaterial);
	//normal->AddTexture(ResourceManager::GetInstance().GetTexture("floor_NRM.dds"));
	//boxMaterial->AddMaterialNode(normal);
	//boxMaterial->SetNormal(normal);

	ScalarNode* roughness = new ScalarNode(boxMaterial);
	roughness->R = 0.2456f;
	//TextureSamplerNode* roughness = new TextureSamplerNode(boxMaterial);
	//roughness->AddTexture(ResourceManager::GetInstance().GetTexture("TexturesCom_Marble_SlabRed_1K_roughness.dds"));
	boxMaterial->AddMaterialNode(roughness);
	boxMaterial->SetRougness(roughness);
 
	planeMaterial = new Material();
	planeMaterial->SetSpecularColor(0.5f, 0.5f, 0.5f);
	planeMaterial->SetSpecularPower(20.0f);

	Vector3Node* albedoPlane = new Vector3Node(planeMaterial);
	albedoPlane->SetValues(0.9f, 0.9f, 0.9f);

	ScalarNode* roughnessPlane = new ScalarNode(planeMaterial);
	roughnessPlane->R = 1.0f;

	planeMaterial->AddMaterialNode(albedoPlane);
	planeMaterial->SetAlbedo(albedoPlane);

	planeMaterial->AddMaterialNode(roughnessPlane);
	planeMaterial->SetRougness(roughnessPlane);

	MaterialCompiler compiler;
	compiler.CompileMaterial(boxMaterial);
	compiler.CompileMaterial(planeMaterial);
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

	factory->CreateSphere(3.0f, 60, 30, sphereData);

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
	//box->SetPixelShader(pixelShader);
	box->SetRenderData(boxRenderData);

	box->SetPosition(-4, 2.5f, -10);
	box->SetRotation(0, 30, 0);
	box->SetMaterial(boxMaterial);

	box2 = new StaticMesh();

	RenderData* box2RenderData = new RenderData();
	box2RenderData->SetVertexBuffer(vertexBuffer);
	box2RenderData->SetIndexBuffer(indexBuffer);
	box2RenderData->SetNumIndices(boxData.indices.size());

	box2->SetVertexShader(vertextShader);
	//box2->SetPixelShader(pixelShader);
	box2->SetRenderData(box2RenderData);

	box2->SetPosition(4, 2.5f, 4);
	box2->SetRotation(0, 52, 0);
	box2->SetMaterial(boxMaterial);

	//  ---- plane ----- //

	plane = new StaticMesh();

	RenderData* planeRenderData = new RenderData();
	planeRenderData->SetVertexBuffer(planeVertexBuffer);
	planeRenderData->SetIndexBuffer(planeIndexBuffer);
	planeRenderData->SetNumIndices(planeData.indices.size());

	plane->SetVertexShader(vertextShader);
	//plane->SetPixelShader(pixelShader);
	plane->SetRenderData(planeRenderData);

	plane->SetPosition(0, 0, -5);
	plane->SetMaterial(planeMaterial);

	//  ---- sphere ----- //

	sphere = new StaticMesh();

	RenderData* sphereRenderData = new RenderData();
	sphereRenderData->SetVertexBuffer(sphereVertexBuffer);
	sphereRenderData->SetIndexBuffer(sphereIndexBuffer);
	sphereRenderData->SetNumIndices(sphereData.indices.size());

	sphere->SetVertexShader(vertextShader);
	//sphere->SetPixelShader(pixelShader);
	sphere->SetRenderData(sphereRenderData);

	sphere->SetPosition(-4.0, 3.0f, 3.0f);
	sphere->SetRotation(90.0f, 110.0f, 0.0f);
	sphere->SetMaterial(boxMaterial);

	world->AddStaticMesh(box);
	world->AddStaticMesh(box2);
	world->AddStaticMesh(plane);
	world->AddStaticMesh(sphere);

	delete factory;
}

void RenderLab::CreateCamera() {
	camera = new CameraComponent();
	camera->SetFieldOfView(3.14f / 4.0f);
	camera->SetAspectratio(1280.0f / 720.0f);
	camera->SetNearPlane(1);
	camera->SetFarPlane(1000);

	camera->SetPosition(0.0f, 30.0f, 20.0f);
	camera->SetCameraTarget(0, 0, 0);

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
	light->SetDirection(0.0f, -0.4f, -1.0f);
	light->SetLightColor(1.0f, 1.0f, 1.0f);
	light->SetBrightness(0.0f);
	light->SetIsEnabled(true);
	light->SetCastsShadows(false);

	pointLight = new PointLightComponent();
	pointLight->SetPosition(-15, 6, 15);
	pointLight->SetAttenuation(0, 0.3f, 0);
	pointLight->SetLightColor(0.6f, 0.6f, 0.6f);
	pointLight->SetRange(100.0f);
	pointLight->SetBrightness(1.0f);
	pointLight->SetIsEnabled(true);
	pointLight->SetCastsShadows(true);

	spotLight = new SpotLightComponent();
	spotLight->SetPosition(12, 10, 12);
	spotLight->SetDirection(-0.5f, -0.5f, -0.7f);
	spotLight->SetAttenuation(0, 0.3f, 0);
	spotLight->SetLightColor(0.2f, 0.2f, 0.7f);
	spotLight->SetRange(100.0f);
	spotLight->SetBrightness(1.0f);
	spotLight->SetConeAngle(22);
	spotLight->SetPenumbraAngle(10);
	spotLight->SetIsEnabled(true);
	spotLight->SetCastsShadows(true);

	spotLight2 = new SpotLightComponent();
	spotLight2->SetPosition(-7, 10, 12);
	spotLight2->SetDirection(0.5f, -0.5f, -0.7f);
	spotLight2->SetAttenuation(0, 0.3f, 0);
	spotLight2->SetLightColor(0.6f, 0.6f, 0.6f);
	spotLight2->SetRange(100.0f);
	spotLight2->SetBrightness(1.0f);
	spotLight2->SetConeAngle(22);
	spotLight2->SetPenumbraAngle(10);
	spotLight2->SetIsEnabled(true);
	spotLight2->SetCastsShadows(true);

	//world->AddDirectionalLight(light);
	world->AddPointLight(pointLight);
	world->AddSpotLight(spotLight);
	world->AddSpotLight(spotLight2);
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
	renderer->AllocateShadowRenderTargets(world);
}

void RenderLab::ShutDown() {

}
