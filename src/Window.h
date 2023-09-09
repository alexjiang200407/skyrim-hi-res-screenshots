#pragma once

#include "WinApi.h"
#include "HRSException.h"
#include "Resolution.h"
#include "Singleton.h"

#define HRS_LAST_EXCEPT HRS::Window::HrException(__LINE__, __FILE__, GetLastError()); 
#define THROW_HRS_LAST_EXCEPT(condition) if (!condition) throw HRS_LAST_EXCEPT

namespace HRS
{
	class Window
		:
		public Singleton<Window>
	{
	public:
		class HrException
			:
			public HRSException
		{
		public:
			HrException(int line, const char* file, HRESULT hr)
				:
				HRSException(line, file), hr(hr)
			{};

			const char*         what() const override;
			virtual const char* GetType() const override { return "Window Exception"; }
			HRESULT             GetErrorCode() const { return hr; };
			std::string         GetDescription() const { return std::system_category().message(hr); };

		private:
			HRESULT hr;
		};

	public:
		static LRESULT CALLBACK WndProc_Hook(
			HWND   hWnd,
			UINT   uMsg,
			WPARAM wParam,
			LPARAM lParam
		);
	public:
		Window()
			:
			startingResolution(GetWindowResolution())
		{}

		~Window() = default;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void                 ScaleWindow(Resolution res);
		static Resolution    GetWindowResolution();
		static HWND          GetHwnd() { return reinterpret_cast<HWND__*>(RE::Main::GetSingleton()->wnd); }
		static HINSTANCE     GetInstance() { return reinterpret_cast<HINSTANCE__*>(RE::Main::GetSingleton()->instance); }
		void                 Init();

	public:
		const Resolution startingResolution;

		WNDPROC          prevWndProc;
	};

	
	
}