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
	private:
		struct Hooks
		{

			struct TakeScreenshot
			{
				static INT32 thunk(ID3D11Texture2D* a_texture_2d, char const* a_path, RE::BSGraphics::TextureFileFormat a_format);
				static inline REL::Relocation<decltype(thunk)> func;
			};

			struct WriteScreenshot
			{
				static INT64 thunk(INT64 a1, UINT32 a2, INT64 a3, const wchar_t* dest);
				static inline REL::Relocation<decltype(thunk)> func;
			};


			static void InstallHooks()
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
	public:
	};
}
