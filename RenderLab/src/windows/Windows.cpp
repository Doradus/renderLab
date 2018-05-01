#include "RenderLab.h"


HWND mainWindowHandle = 0;

bool InitWindowsApp(HINSTANCE instanceHandle, int show);

int Run();

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
RenderLab* renderLab;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) {
	if (!InitWindowsApp(hInstance, nShowCmd)) {
		return 0;
	}

	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show) {
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(55, 55, 55));;
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"FusionRenderEngine";

	if (!RegisterClass(&wc)) {
		MessageBox(0, L"RegisterClass FAIELD", 0, 0);
		return false;
	}

	mainWindowHandle = CreateWindow(
		L"FusionRenderEngine",
		L"FusionRenderEngine",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		instanceHandle,
		0);

	if (mainWindowHandle == 0) {
		MessageBox(0, L"CreateWindow FAIELD", 0, 0);
		return false;
	}

	ShowWindow(mainWindowHandle, show);
	UpdateWindow(mainWindowHandle);
	renderLab = new RenderLab(mainWindowHandle);
	renderLab->InitLab();
	return true;
}

int Run() {
	MSG msg = { 0 };
	renderLab->PrepareStart();
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			renderLab->Tick();
			renderLab->Draw();
		}
	}

	return (int)msg.wParam;
}


LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World", L"Hello", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(mainWindowHandle);
			delete renderLab;
			renderLab = nullptr;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		delete renderLab;
		renderLab = nullptr;
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}