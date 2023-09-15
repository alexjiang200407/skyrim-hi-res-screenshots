#include "HRS.h"
#include "Window.h"
#include "logger.h"

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
		!btnEvt->IsPressed() ||
		btnEvt->IsRepeating()
	)
		return RE::BSEventNotifyControl::kContinue;

	uint32_t idCode = btnEvt->GetIDCode();
	
	// Don't handle escape
	if (idCode == 1)
		return RE::BSEventNotifyControl::kContinue;

	if (idCode == inputSettings.GetScreenshotKey())
	{
		Window::GetSingleton()->gfx->CaptureScreen();
	}
	else if (idCode == inputSettings.GetUpscaleKey())
	{
		if (upscaled)
		{
			Window::GetSingleton()->ScaleWindow(Window::GetSingleton()->startingResolution);
		}
		else
		{
			Window::GetSingleton()->ScaleWindow({ 2560, 1440 });
		}

		upscaled = !upscaled;
	}

	return RE::BSEventNotifyControl::kContinue;
}

uint32_t HRS::HiResScreenshots::InputEventSettings::GetScreenshotKey() 
{ 
	std::string val = GetSetting("screenshotKey");
	return strtoul(val.c_str(), nullptr, 0);
}

uint32_t HRS::HiResScreenshots::InputEventSettings::GetUpscaleKey()
{
	std::string val = GetSetting("upscaleKey");
	return strtoul(val.c_str(), nullptr, 0);
}
