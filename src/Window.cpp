#include "Window.h"

void HRS::Window::ScaleWindow(Resolution res)
{
	try
	{
		THROW_HRS_LAST_EXCEPT(SetWindowPos(
			GetHwnd(), nullptr, 0, 0,
			res.width, res.height,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
		));
	}
	catch (const HRSException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "Unknown Exception", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
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

