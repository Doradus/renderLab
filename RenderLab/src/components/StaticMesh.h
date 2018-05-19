#pragma once
#include "RenderingResources.h"
#include "SceneComponent.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderData.h"
#include "Material.h"

class StaticMesh : public SceneComponent {
public:
	StaticMesh();
	~StaticMesh();

	VertexShader*	GetVertexShader() const;
	PixelShader*	GetPixelShader() const;
	RenderData*		GetRenderData() const;
	Material*		GetMaterial() const;

	void			SetVertexShader(VertexShader* shader);
	void			SetPixelShader(PixelShader* shader);
	void			SetRenderData(RenderData* inData);
	void			SetMaterial(Material* inMaterial);

private:
	VertexShader*	vertexShader;
	PixelShader*	pixelShader;
	RenderData*		renderData;
	Material*		material;
};