#include "Window.h"
#include "logger.h"

LRESULT CALLBACK HRS::Window::WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_WINDOWPOSCHANGED)
	{
		logger::info("Finished scaling");
	}
	else if (uMsg == WM_WINDOWPOSCHANGING)
	{
		logger::info("Started scaling");
	}

	return CallWindowProcW(Window::GetSingleton()->prevWndProc, Window::GetSingleton()->GetHwnd(), uMsg, wParam, lParam);
}

void HRS::Window::ScaleWindow(Resolution res)
{
	THROW_HRS_LAST_EXCEPT(SetWindowPos(
		GetHwnd(), nullptr, 0, 0,
		res.width, res.height,
		//SWP_NOMOVE | SWP_NOREPOSITION |
		SWP_ASYNCWINDOWPOS
	));
}

HRS::Resolution HRS::Window::GetWindowResolution()
{
	RECT rect;
	THROW_HRS_LAST_EXCEPT(
		GetWindowRect(GetHwnd(), &rect)
	);

	return Resolution{ rect.right - rect.left, rect.bottom - rect.top };
}

void HRS::Window::Init()
{
	prevWndProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(
			GetHwnd(),
			GWLP_WNDPROC,
			LONG_PTR(WndProc_Hook)
		));

	THROW_HRS_LAST_EXCEPT(prevWndProc);

	logger::info("Installed Window processer hook.");
}


const char* HRS::Window::HrException::what() const
{
	std::ostringstream oss;
	oss << GetType() << '\n'
		<< "Error Code: 0x" << std::hex << GetErrorCode() << '\n'
		<< GetDescription() << '\n'
		<< GetOrigin() << '\n';

	buffer = oss.str();

	return buffer.c_str();
}