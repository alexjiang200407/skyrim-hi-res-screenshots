#include "Window.h"
#include "logger.h"
#include "ScreenshotHandler.h"

LRESULT CALLBACK HRS::Window::WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto* wnd = Window::GetSingleton();

#ifndef NDEBUG
	{
		std::string msgStr = wnd->msgMap.Decode(uMsg);
		logger::info("{} wp: {:x}, lp: {:x}", msgStr, wParam, lParam);
	}
#endif

	switch (uMsg)
	{
	case WM_WINDOWPOSCHANGED:
		return DefWindowProc(wnd->GetHwnd(), uMsg, wParam, lParam);

	case WM_WINDOWPOSCHANGING:
		return DefWindowProc(wnd->GetHwnd(), uMsg, wParam, lParam);

	case WM_SIZE:
	{
		wnd->gfx->ResizeBuffer({ static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)) });
		break;
	}
	}

	return CallWindowProcW(Window::GetSingleton()->prevWndProc, hWnd, uMsg, wParam, lParam);
}

HRS::Window::~Window()
{
	delete gfx;
}


HRS::Resolution HRS::Window::GetWindowResolution()
{
	RECT rect;
	THROW_HRS_LAST_EXCEPT(
		GetWindowRect(Window::GetSingleton()->GetHwnd(), &rect)
	);

	return Resolution{ rect.right - rect.left, rect.bottom - rect.top };
}

void HRS::Window::ScaleWindow(Resolution res)
{
	logger::info("Scaling window to {} {}", res.width, res.height);
	THROW_HRS_LAST_EXCEPT(SetWindowPos(GetHwnd(), HWND_TOP, 0, 0, res.width, res.height, SWP_ASYNCWINDOWPOS));
}

void HRS::Window::Register(HWND hWnd, RE::BSGraphics::RendererWindow renderWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	logger::info("Registering the window");

	wnd = hWnd;
	startingResolution = { renderWnd.windowWidth, renderWnd.windowHeight };

	logger::info("Starting resolution {} {}", startingResolution.width, startingResolution.height);
	gfx = new Graphics{ hWnd, device, context, renderWnd.swapChain };

	prevWndProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(
			hWnd,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(Window::WndProc_Hook)
		));

	THROW_HRS_LAST_EXCEPT(prevWndProc);
	registered = true;

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