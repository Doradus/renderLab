#pragma once
#include <vector>
#include "StaticMesh.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "CameraComponent.h"

class World {
public:
	World();
	~World();

	void AddStaticMesh(const StaticMesh* mesh);
	void AddDirectionalLight(const DirectionalLightComponent* light);
	void AddPointLight(const PointLightComponent* light);
	void AddCamera(const CameraComponent* camera);

	std::vector<const StaticMesh*> GetAllStaticMeshes() const;
	std::vector<const DirectionalLightComponent*> GetAllDirectionalLights() const;
	std::vector<const PointLightComponent*> GetAllPointLights() const;
	const CameraComponent* GetActiveCamera() const;

private:
	std::vector<const StaticMesh*> staticMeshes;
	std::vector<const DirectionalLightComponent*> directionalLight;
	std::vector<const PointLightComponent*> pointLights;
	const CameraComponent* camera;
};