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
		static constexpr size_t numSettings = 1;
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

			constexpr std::array<std::string, numSettings> GetAllKeys() const { return keys; };
			constexpr const char*                          GetSection() const { return section; };
			uint32_t                                       GetScreenshotKey();

		private:
			static constexpr const char*               section = "Input";
			const std::array<std::string, numSettings> keys = { "screenshotKey"s };

		};
	public:
		void                     Register();
		RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*) override;


	private:
		InputEventSettings inputSettings{ ".\\Data\\SKSE\\Plugins\\HiResScreenshots.ini" };
	};
}

