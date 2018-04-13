#include "D3D11Utils.h"
#include <comdef.h>
#include "Logging.h"

void VerifyD3DResult(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumbere) {
	_com_error err(hr);
	std::wstring errorMasg = err.ErrorMessage();

	LOG_ERROR((functionName + L" faild in " + filename + L" on line " + std::to_wstring(lineNumbere) + L" with error: " + errorMasg).c_str());
}

