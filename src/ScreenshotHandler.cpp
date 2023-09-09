#include "ScreenshotHandler.h"
#include "logger.h"

void HRS::ScreenshotHandler::HiResScreenshot()
{
	logger::info("Screenshot started");

	if (HRS::Window::GetSingleton()->startingResolution == HRS::Window::GetSingleton()->GetWindowResolution())
	{
		HRS::Window::GetSingleton()->ScaleWindow(settings.GetScreenshotResolution());


		//const char* f = "C:\\Users\\alexj\\OneDrive\\Documents\\My Games\\Screenshots\\Poo.png";

		//Hooks::TakeScreenshot::thunk(0, 0, f, 3);
	}
	else
	{
		HRS::Window::GetSingleton()->ScaleWindow(HRS::Window::GetSingleton()->startingResolution);
	}
}

RE::BSEventNotifyControl HRS::ScreenshotHandler::ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*)
{
	if (!event || !(*event) || (*event)->eventType != RE::INPUT_EVENT_TYPE::kButton)
		return RE::BSEventNotifyControl::kContinue;

	RE::ButtonEvent* btnEvt = (*event)->AsButtonEvent();

	if (
		btnEvt->GetIDCode() == 38 &&
		btnEvt->IsPressed() && 
		!btnEvt->IsRepeating()
	)
	{
		try
		{
			HiResScreenshot();
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

	return RE::BSEventNotifyControl::kContinue;
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
	RE::BSInputDeviceManager::GetSingleton()->AddEventSink<RE::InputEvent*>(this);
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
