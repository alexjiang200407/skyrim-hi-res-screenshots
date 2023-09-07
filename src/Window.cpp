#include "Window.h"

void HRS::Window::ScaleWindow(Resolution res)
{
	THROW_HRS_LAST_EXCEPT(SetWindowPos(
		GetHwnd(), nullptr, 0, 0,
		res.width, res.height,
		SWP_NOMOVE | SWP_NOZORDER |
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

