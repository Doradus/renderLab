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
	void AddDirectionalLight(const DirectionalLightComponent* light);
	void AddPointLight(const PointLightComponent* light);
	void AddSpotLight(SpotLightComponent* light);
	void AddCamera(const CameraComponent* camera);

	std::vector<const StaticMesh*> GetAllStaticMeshes() const;
	std::vector<const DirectionalLightComponent*> GetAllDirectionalLights() const;
	std::vector<const PointLightComponent*> GetAllPointLights() const;
	std::vector<SpotLightComponent*> GetAllSpotLights() const;
	TextureRI* GetShadowMap() const;
	TextureRI* GetShadowMapCube() const;
	const CameraComponent* GetActiveCamera() const;

private:
	void CreateShadowMap();
	void CreateShadowMapCube();
	std::vector<const StaticMesh*> staticMeshes;
	std::vector<const DirectionalLightComponent*> directionalLight;
	std::vector<const PointLightComponent*> pointLights;
	std::vector<SpotLightComponent*> spotLights;
	TextureRI* shadowMap;
	TextureRI* shadowMapCube;
	const CameraComponent* camera;
};