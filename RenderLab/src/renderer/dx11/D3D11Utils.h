#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <string>

extern void VerifyD3DResult(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumbere);


#define VERIFY_D3D_RESULT(x) {													\
HRESULT hr = x;																	\
	if (FAILED(hr)) {															\
		std::wstring file = AnsiToWString(__FILE__);							\
		VerifyD3DResult(hr,L#x,file,__LINE__);									\
	}																			\
}																				\

#define RELEASE(x) { x->Release(); x = 0;}

inline std::wstring AnsiToWString(const std::string& str) {
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}