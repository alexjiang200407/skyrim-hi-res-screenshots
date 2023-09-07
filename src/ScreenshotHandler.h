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
		public Singleton<ScreenshotHandler>,
		public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		static constexpr size_t numSettings = 2;

	public:
		class ScreenshotSettings
			:
			public Settings<numSettings>
		{
		public:
			ScreenshotSettings() = delete;
			ScreenshotSettings(const char* filename)
				:
				Settings(filename)
			{
				ReadAllSettings();
			}


		public:
			virtual constexpr const char* GetSection() const override { return section; }
			virtual constexpr std::array<std::string, numSettings> GetAllKeys() const override { return keys;  }

		public:
			HRS::Resolution GetScreenshotResolution();



		public:
			static constexpr const char*               section = "Screenshot";
			const std::array<std::string, numSettings> keys = { "screenshotWidth"s, "screenshotHeight"s };

		};

	private:
		struct ScreenshotCallbackParams
		{
			ScreenshotCallbackParams() = delete;
			ScreenshotCallbackParams(
				void (*callback)(HRS::Resolution),
				HRS::Resolution rescale
			)
				: callback(callback), rescale(rescale)
			{}
			
			void (*callback)(HRS::Resolution);
			HRS::Resolution rescale;
		};
	public:
		ScreenshotHandler() = default;
		ScreenshotHandler(const ScreenshotHandler&) = delete;
		const ScreenshotHandler& operator=(const ScreenshotHandler&) = delete;
		
		
		
		void Register();
		void Screenshot();
		RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*) override;

	private:
		static DWORD WINAPI CheckScreenshotCompleted(LPVOID callback);
		static void ScreenshotCompletedCallback(HRS::Resolution res);

	public:
		ScreenshotSettings settings{".\\Data\\SKSE\\Plugins\\HiResScreenshots.ini"};
	};
}
