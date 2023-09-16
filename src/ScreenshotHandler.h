#pragma once
#include "WinAPI.h"
#include "Settings.h"
#include "Resolution.h"
#include "Window.h"
#include "Singleton.h"

namespace HRS
{
	class ScreenshotHandler
		:
		public Singleton<ScreenshotHandler>
	{
	public:
		enum class ImageType
		{
			BMP,
			PNG,
			JPG,
			TIFF
		};

	public:
		static constexpr size_t numSettings = 4;
		class ScreenshotSettings
			:
			public Settings<numSettings>
		{

		public:
			ScreenshotSettings(const char* file)
				:
				Settings(file)
			{
				ReadAllSettings();
			}

			constexpr std::array<Option, numSettings> GetAllOptions() const override
			{
				return {
					Option{ "upscaleWidth", "1920" },
					Option{ "upscaleHeight", "1080" },
					Option{ "screenshotFolder", ".\\"},
					Option{ "imageFormat", "2" }
				};
			};

			constexpr const char*    GetSection() const override { return "Screenshot"; };

			Resolution               GetUpscaleResolution();
			std::string              GetScreenshotFolder();
			ImageType                GetImageType();
		};
	public:
		ScreenshotHandler() = default;
		ScreenshotHandler(const ScreenshotHandler&) = delete;
		const ScreenshotHandler& operator=(const ScreenshotHandler&) = delete;	

	public:
		ScreenshotSettings settings{ INI_FILE };
	};
}
