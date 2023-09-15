#include "ScreenshotHandler.h"
#include "logger.h"
#include <DirectXTex.h>


INT32 HRS::ScreenshotHandler::Hooks::TakeScreenshot::thunk(ID3D11Texture2D* a_texture_2d, char const* a_path, RE::BSGraphics::TextureFileFormat a_format)
{
	Window::GetSingleton()->gfx->CaptureScreen(a_texture_2d);
	a_texture_2d->Release();
	return 0;
}

INT64 HRS::ScreenshotHandler::Hooks::WriteScreenshot::thunk(INT64 a1, UINT32 a2, INT64 a3, const wchar_t* dest)
{
	logger::info("Writing screenshot {} {} {}", a1, a2, a3);
	return func(a1, a2, a3, dest);
}

void HRS::ScreenshotHandler::Register()
{
	Hooks::InstallHooks();
}
