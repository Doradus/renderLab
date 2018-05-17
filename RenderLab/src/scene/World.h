#pragma once
#include <vector>
#include "StaticMesh.h"
#include "DirectionalLightComponent.h"
#include "CameraComponent.h"

class World {
public:
	World();
	~World();

	void AddStaticMesh(const StaticMesh* mesh);
	void AddDirectionalLight(const DirectionalLightComponent* light);
	void AddCamera(const CameraComponent* camera);

	std::vector<const StaticMesh*> GetAllStaticMeshes() const;
	std::vector<const DirectionalLightComponent*> GetAllDirectionalLights() const;
	const CameraComponent* GetActiveCamera() const;

private:
	std::vector<const StaticMesh*> staticMeshes;
	std::vector<const DirectionalLightComponent*> directionalLight;
	const CameraComponent* camera;
};