#pragma once
#include "Settings.h"
#include "Singleton.h"


namespace HRS
{
	class HiResScreenshots
		:
		public Singleton<HiResScreenshots>,
		public RE::BSTEventSink<RE::InputEvent*>
	{
	private:
		static constexpr size_t numSettings = 2;
		class InputEventSettings
			:
			public Settings<numSettings>
		{

		public:
			InputEventSettings() = delete;
			InputEventSettings(const char* fPath)
				:
				Settings(fPath)
			{
				ReadAllSettings();
			}

			constexpr std::array<Option, numSettings>      GetAllKeys() const { return { Option{ "screenshotKey", "38" }, Option{ "upscaleKey", "1"}}; };
			constexpr const char*                          GetSection() const { return "Input"; };
			uint32_t                                       GetScreenshotKey();
			uint32_t                                       GetUpscaleKey();
		};
	public:
		void                     Register();
		RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*) override;


	private:
		bool               upscaled = false;
		InputEventSettings inputSettings{ ".\\Data\\SKSE\\Plugins\\HiResScreenshots.ini" };
	};
}

