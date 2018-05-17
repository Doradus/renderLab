#include "World.h"

World::World() {
	staticMeshes = {};
	directionalLight = {};
}

World::~World() {

}

void World::AddStaticMesh(const StaticMesh* mesh)  {
	staticMeshes.push_back(mesh);
}

void World::AddDirectionalLight(const DirectionalLightComponent* light) {
	directionalLight.push_back(light);
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

const CameraComponent* World::GetActiveCamera() const {
	return camera;
}
