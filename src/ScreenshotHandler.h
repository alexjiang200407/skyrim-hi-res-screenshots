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
			virtual constexpr const char*                          GetSection() const override { return section; }
			virtual constexpr std::array<std::string, numSettings> GetAllKeys() const override { return keys;  }

		public:
			HRS::Resolution GetScreenshotResolution();


		public:
			static constexpr const char*               section = "Screenshot";
			const std::array<std::string, numSettings> keys = { "screenshotWidth"s, "screenshotHeight"s};

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
	
	private:
		struct Hooks
		{

			struct TakeScreenshot
			{
				static INT32 thunk(INT64 a1, INT64 a2, const char* dest, UINT32 type);
				static inline REL::Relocation<decltype(thunk)> func;
			};

			struct WriteScreenshot
			{
				static INT64 thunk(INT64 a1, UINT32 a2, INT64 a3, const wchar_t* dest);
				static inline REL::Relocation<decltype(thunk)> func;
			};


			static void Install()
			{
				REL::Relocation<std::uintptr_t> TakeScreenshot_hook{ REL::ID(36853), 0x69 };
				REL::Relocation<std::uintptr_t> WriteScreenshot_hook{ REL::ID(77406), 0x143 };

				stl::write_thunk_call<TakeScreenshot>(TakeScreenshot_hook.address());
				stl::write_thunk_call<WriteScreenshot>(WriteScreenshot_hook.address());
			}
		};
	
	public:
		ScreenshotHandler() = default;
		ScreenshotHandler(const ScreenshotHandler&) = delete;
		const ScreenshotHandler& operator=(const ScreenshotHandler&) = delete;	
		
		
		void Register();
		void HiResScreenshot();
		RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*) override;

	public:
		ScreenshotSettings settings{".\\Data\\SKSE\\Plugins\\HiResScreenshots.ini"};

	};
}
