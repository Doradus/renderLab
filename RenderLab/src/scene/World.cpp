#include "World.h"

World::World() : 
shadowMap (nullptr)
{
	staticMeshes = {};
	directionalLight = {};
}

World::~World() {
	if (shadowMap) {
		delete shadowMap;
		shadowMap = nullptr;
	}
}

void World::AddStaticMesh(const StaticMesh* mesh)  {
	staticMeshes.push_back(mesh);
}

void World::AddDirectionalLight(const DirectionalLightComponent* light) {
	directionalLight.push_back(light);
	CreateShadowMap();
}

void World::AddPointLight(const PointLightComponent * light) {
	pointLights.push_back(light);
}

void World::AddSpotLight(SpotLightComponent * light) {
	spotLights.push_back(light);
	CreateShadowMap();
}

void World::AddCamera(const CameraComponent* inCamera) {
	camera = inCamera;
}

std::vector<const StaticMesh*> World::GetAllStaticMeshes() const {
	return staticMeshes;
}

std::vector<const DirectionalLightComponent*> World::GetAllDirectionalLights() const {
	return directionalLight;
}

std::vector<const PointLightComponent*> World::GetAllPointLights() const {
	return pointLights;
}

std::vector<SpotLightComponent*> World::GetAllSpotLights() const {
	return spotLights;
}

TextureRI * World::GetShadowMap() const {
	return shadowMap;
}

const CameraComponent* World::GetActiveCamera() const {
	return camera;
}

void World::CreateShadowMap() {
	shadowMap = renderingInterface->CreateTexture2d(1024, 1024, 1, SHADOW_DEPTH, TextureBindAsDepthStencil | TextureBindAsShaderResource, 1);
}
