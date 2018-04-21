#pragma once

#include "RenderingInterface.h"
#include "D3D11RenderingInterface.h"

class RenderLab {
public:
	RenderLab(HWND windowHandle);
	~RenderLab();
	RenderLab(const RenderLab& that) = delete;
	RenderLab& operator = (const RenderLab&) = delete;

	bool		InitLab();
	void		ShutDown();

private:
	bool CreateRenderingInterface();
	void InitShaders();
	RenderingInterface* renderingInterface;
	HWND windowHandle;
};