#include "ScreenshotHandler.h"


void HRS::ScreenshotHandler::Screenshot(void (*callback)()) const
{
	RE::MenuControls::GetSingleton()->QueueScreenshot();

	DWORD threadId;
	HANDLE handle = CreateThread(0, 0, CheckScreenshotCompleted, callback, 0, &threadId);
}

DWORD WINAPI HRS::ScreenshotHandler::CheckScreenshotCompleted(LPVOID callback)
{
	auto& screenshotHandler = RE::MenuControls::GetSingleton()->screenshotHandler;
	while (screenshotHandler->screenshotQueued);

	((void (*)())(callback))();

	return 0;
}


