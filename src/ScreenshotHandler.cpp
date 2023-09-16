#include "ScreenshotHandler.h"
#include "logger.h"
#include <DirectXTex.h>


HRS::Resolution HRS::ScreenshotHandler::ScreenshotSettings::GetUpscaleResolution()
{
	std::string buffer = GetSetting("upscaleWidth");
	int width = atoi(buffer.c_str());

	buffer = GetSetting("upscaleHeight");

	int height = atoi(buffer.c_str());

	return Resolution(width, height);
}

std::string HRS::ScreenshotHandler::ScreenshotSettings::GetScreenshotFolder()
{
	std::string str = GetSetting("screenshotFolder");

	return str;
}

HRS::ScreenshotHandler::ImageType HRS::ScreenshotHandler::ScreenshotSettings::GetImageType()
{
	std::string buffer = GetSetting("imageFormat");
	return (ImageType)atoi(buffer.c_str());
}
