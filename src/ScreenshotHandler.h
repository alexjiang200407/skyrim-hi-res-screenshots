#pragma once
#include "WinAPI.h"
#include "Settings.h"

namespace HRS
{
	class ScreenshotHandler
	{
	public:
		class ScreenshotSettings
			:
			public Settings<2>
		{
			constexpr const char* GetSection() const override { return section; }
			constexpr std::array<std::string, 2> GetAllKeys() const override { return keys;  }

		public:
			static constexpr size_t numSettings = 2;
			static constexpr const char* section = "Screenshot";
			const std::array<std::string, 2> keys = { "screenshotWidth"s, "screenshotHeight"s };

		};
	public:
		ScreenshotHandler() = default;
		void Screenshot(void (*callback)()) const;

	private:
		static DWORD WINAPI CheckScreenshotCompleted(LPVOID callback);
	};
}
