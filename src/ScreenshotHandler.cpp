#include "ScreenshotHandler.h"
#include "logger.h"

void HRS::ScreenshotHandler::TakeScreenshot()
{
	const char* f = "C:\\Users\\alexj\\OneDrive\\Documents\\My Games\\Screenshots\\Poo.png";
	
	Hooks::TakeScreenshot::thunk(0, 0, f, 3);
}

HRS::Resolution HRS::ScreenshotHandler::ScreenshotSettings::GetScreenshotResolution()
{
	int width = atoi(GetSetting("screenshotWidth").c_str());
	int height = atoi(GetSetting("screenshotHeight").c_str());
	return { width, height };
}


void HRS::ScreenshotHandler::Register()
{
	ScreenshotHandler::Hooks::Install();
}

INT32 HRS::ScreenshotHandler::Hooks::TakeScreenshot::thunk(INT64 a1, INT64 a2, const char* dest, UINT32 type)
{
	HRS::Resolution res = HRS::Window::GetWindowResolution();
	logger::info("Taking screenshot {} {} {} {}. Resolution: {} {}", a1, a2, dest, type, res.width, res.height);
	return func(a1, a2, dest, type);
}

INT64 HRS::ScreenshotHandler::Hooks::WriteScreenshot::thunk(INT64 a1, UINT32 a2, INT64 a3, const wchar_t* dest)
{
	logger::info("Writing screenshot {} {} {}", a1, a2, a3);
	return func(a1, a2, a3, dest);
}
