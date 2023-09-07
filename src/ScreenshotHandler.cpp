#include "ScreenshotHandler.h"


void HRS::ScreenshotHandler::Screenshot()
{
	Resolution originalResolution = HRS::Window::GetSingleton()->GetWindowResolution();
	HRS::Window::GetSingleton()->ScaleWindow(settings.GetScreenshotResolution());


	RE::MenuControls::GetSingleton()->QueueScreenshot();

	ScreenshotCallbackParams* params = new ScreenshotCallbackParams {
		ScreenshotCompletedCallback, originalResolution
	};

	DWORD threadId;
	HANDLE handle = CreateThread(
		0, 0, CheckScreenshotCompleted, params,
		0, &threadId
	);

	THROW_HRS_LAST_EXCEPT(handle);
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
			Screenshot();
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

DWORD WINAPI HRS::ScreenshotHandler::CheckScreenshotCompleted(LPVOID params)
{
	auto& screenshotHandler = RE::MenuControls::GetSingleton()->screenshotHandler;
	while (screenshotHandler->screenshotQueued);

	ScreenshotCallbackParams* callbackParams = (ScreenshotCallbackParams*)params;
	(callbackParams)->callback(callbackParams->rescale);

	delete callbackParams;

	return 0;
}

void HRS::ScreenshotHandler::ScreenshotCompletedCallback(HRS::Resolution res)
{
	HRS::Window::GetSingleton()->ScaleWindow(res);
	RE::UIMessageQueue* msgQueue = RE::UIMessageQueue::GetSingleton();

	for (auto& menu : RE::UI::GetSingleton()->menuMap)
	{
		const char *str = menu.first.c_str();
		msgQueue->AddMessage(menu.first, RE::UI_MESSAGE_TYPE::kUpdate, nullptr);
	}

	msgQueue->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kUpdate, nullptr);
}

HRS::Resolution HRS::ScreenshotHandler::ScreenshotSettings::GetScreenshotResolution()
{
	int width = atoi(GetSetting("screenshotWidth").c_str());
	int height = atoi(GetSetting("screenshotHeight").c_str());
	return { width, height };
}

void HRS::ScreenshotHandler::Register()
{
	RE::BSInputDeviceManager::GetSingleton()->AddEventSink<RE::InputEvent*>(this);
}
