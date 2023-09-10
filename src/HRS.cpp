#include "HRS.h"
#include "Window.h"

void HRS::HiResScreenshots::Register()
{
	RE::BSInputDeviceManager::GetSingleton()->AddEventSink<RE::InputEvent*>(this);
}

RE::BSEventNotifyControl HRS::HiResScreenshots::ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*)
{
	if (!event || !(*event) || (*event)->eventType != RE::INPUT_EVENT_TYPE::kButton)
		return RE::BSEventNotifyControl::kContinue;

	RE::ButtonEvent* btnEvt = (*event)->AsButtonEvent();

	if (
		btnEvt->GetIDCode() == RE::DirectInput8::DIK_L &&
		btnEvt->IsPressed() &&
		!btnEvt->IsRepeating()
		)
	{
		try
		{
			HRS::Window::GetSingleton()->TakeScreenshot();
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

uint32_t HRS::HiResScreenshots::InputEventSettings::GetScreenshotKey() 
{ 
	const char* val = GetSetting("screenshotKey").c_str(); 
	return atoll(val);
}
