#include "World.h"

World::World()
{
	staticMeshes = {};
	directionalLight = {};
}

World::~World() {
}

void World::AddStaticMesh(const StaticMesh* mesh)  {
	staticMeshes.push_back(mesh);
}

void World::AddDirectionalLight(DirectionalLightComponent* light) {
	directionalLight.push_back(light);
}

void World::AddPointLight(PointLightComponent * light) {
	pointLights.push_back(light);
}

void World::AddSpotLight(SpotLightComponent * light) {
	spotLights.push_back(light);
}

void World::AddCamera(const CameraComponent* inCamera) {
	camera = inCamera;
}

std::vector<const StaticMesh*> World::GetAllStaticMeshes() const {
	return staticMeshes;
}

std::vector<DirectionalLightComponent*> World::GetAllDirectionalLights() const {
	return directionalLight;
}

std::vector<PointLightComponent*> World::GetAllPointLights() const {
	return pointLights;
}

std::vector<SpotLightComponent*> World::GetAllSpotLights() const {
	return spotLights;
}

const CameraComponent* World::GetActiveCamera() const {
	return camera;
}
