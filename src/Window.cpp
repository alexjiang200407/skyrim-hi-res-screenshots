#include "Window.h"
#include "logger.h"
#include "ScreenshotHandler.h"

LRESULT CALLBACK HRS::Window::WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto* wnd = Window::GetSingleton();	

#ifdef _DEBUG
	{
		std::string msgStr = wnd->msgMap.Decode(uMsg);
		logger::info("{} wp: {:x}, lp: {:x}", msgStr, wParam, lParam);
	}
#endif

	//if (uMsg == WM_WINDOWPOSCHANGING && wnd->startingResolution != wnd->GetWindowResolution())
	//{
	//	logger::info("Window pos changed! Resolution: {} {}", wnd->GetWindowResolution().width, wnd->GetWindowResolution().height);
	//	ScreenshotHandler::GetSingleton()->TakeScreenshot();
	//}
	//else if (uMsg == WM_ERASEBKGND && wnd->startingResolution != wnd->GetWindowResolution())
	//{
	//	logger::info("erasing background cancelled");
	//	return 0;
	//}

	return CallWindowProcW(Window::GetSingleton()->prevWndProc, hWnd, uMsg, wParam, lParam);
}


void HRS::Window::ScaleWindow(Resolution res)
{
	THROW_HRS_LAST_EXCEPT(SetWindowPos(
		GetHwnd(), HWND_TOP, 0, 0,
		res.width, res.height,
		SWP_ASYNCWINDOWPOS | SWP_NOCOPYBITS
	));
}

HRS::Resolution HRS::Window::GetWindowResolution()
{
	RECT rect;
	THROW_HRS_LAST_EXCEPT(
		GetWindowRect(Window::GetSingleton()->GetHwnd(), &rect)
	);

	return Resolution{ rect.right - rect.left, rect.bottom - rect.top };
}

void HRS::Window::TakeScreenshot()
{
	if (Window::GetSingleton()->GetWindowResolution() == Window::GetSingleton()->startingResolution)
	{
		ScaleWindow(ScreenshotHandler::GetSingleton()->settings.GetScreenshotResolution());
	}
	else
	{
		ScaleWindow(Window::GetSingleton()->startingResolution);
	}
}

void HRS::Window::Register(HWND hWnd)
{
	logger::info("Registering the window");

	wnd = hWnd;

	//{
	//	RECT rect{};
	//	THROW_HRS_LAST_EXCEPT(GetWindowRect(wnd, reinterpret_cast<LPRECT>(&rect)));
	//	startingResolution = HRS::Resolution{ rect.right - rect.left, rect.bottom - rect.top };
	//}


	logger::info("Starting resolution {} {}", startingResolution.width, startingResolution.height);


	prevWndProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(
			hWnd,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(Window::WndProc_Hook)
		));


	THROW_HRS_LAST_EXCEPT(prevWndProc);

	logger::info("Successfully installed Window processer hook.");
}

void HRS::Window::Unregister()
{
	THROW_HRS_LAST_EXCEPT(SetWindowLongPtr(
		GetHwnd(),
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(prevWndProc)
	));
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