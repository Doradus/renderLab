#include "RenderLab.h"

RenderLab::RenderLab(HWND window) :
	windowHandle(window) {
};

RenderLab::~RenderLab() {
	delete renderingInterface;
	renderingInterface = nullptr;
};

bool RenderLab::InitLab() {
	if (!CreateRenderingInterface()) {
		return false;
	}

	return true;
};

bool RenderLab::CreateRenderingInterface() {
	renderingInterface = new D3D11RenderingInterface(500, 500, windowHandle);
	renderingInterface->InitRenderer();
	return true;
}

void RenderLab::ShutDown() {

}
