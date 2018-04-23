#pragma once
#include "RenderUtils.h"

class StaticMesh {
public:
	StaticMesh();
	~StaticMesh();

	void		SetPosition(XMFLOAT3 inPosition);
	void		SetPosition(float x, float y, float z);
	void		SetScale(float inScale);
	void		SetScale(XMFLOAT3 inScale);
	void		SetScale(float x, float y, float z);
	void		SetRotation(XMFLOAT3 inRotation);
	void		SetRotation(float x, float y, float z);
	void		Move(float x, float y, float z);

	void		UpdateTransform();

	XMFLOAT4X4	GetWorld() const;

private:
	bool		isDirty;
	XMFLOAT3	scale;
	XMFLOAT3	rotation;
	XMFLOAT3	position;
	XMFLOAT4X4	worldMatrix;
};