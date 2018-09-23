#pragma once
#include <vector>
#include "StaticMesh.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "CameraComponent.h"

class World {
public:
	World();
	~World();

	void AddStaticMesh(const StaticMesh* mesh);
	void AddDirectionalLight(DirectionalLightComponent* light);
	void AddPointLight(PointLightComponent* light);
	void AddSpotLight(SpotLightComponent* light);
	void AddCamera(const CameraComponent* camera);

	std::vector<const StaticMesh*> GetAllStaticMeshes() const;
	std::vector<DirectionalLightComponent*> GetAllDirectionalLights() const;
	std::vector<PointLightComponent*> GetAllPointLights() const;
	std::vector<SpotLightComponent*> GetAllSpotLights() const;
	const CameraComponent* GetActiveCamera() const;

private:
	void CreateShadowMap();
	void CreateShadowMapCube();
	std::vector<const StaticMesh*> staticMeshes;
	std::vector<DirectionalLightComponent*> directionalLight;
	std::vector<PointLightComponent*> pointLights;
	std::vector<SpotLightComponent*> spotLights;
	const CameraComponent* camera;
};