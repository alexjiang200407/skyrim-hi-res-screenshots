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
	public:
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

			constexpr std::array<Option, numSettings>      GetAllOptions() const
			{ 
				return {
					Option{ "screenshotKey", "210" }, 
					Option{ "upscaleKey", "1" }
				}; 
			}
			constexpr const char*                          GetSection() const { return "Input"; };
			uint32_t                                       GetScreenshotKey();
			uint32_t                                       GetUpscaleKey();
		};
	public:
		void                     Register();
		RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*) override;
		static void              RereadAllSettings();


	public:
		InputEventSettings inputSettings{ INI_FILE };

	private:
		bool               upscaled = false;
	};
}

