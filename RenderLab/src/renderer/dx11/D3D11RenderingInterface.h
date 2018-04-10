#pragma once
#include "D3D11Utils.h"
#include "RenderingInterface.h"

class D3D11RenderingInterface : public RenderingInterface {
public :
	D3D11RenderingInterface(HWND mainWindow);
	virtual ~D3D11RenderingInterface();
	D3D11RenderingInterface(const D3D11RenderingInterface& that) = delete;

	virtual void InitRenderer() final;
};