#pragma once

#include "WinApi.h"
#include "HRSException.h"
#include "Resolution.h"
#include "Singleton.h"
#include "WindowsMessageMap.h"
#include "Graphics.h"

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
		Window() = default;
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void                            Register(HWND hWnd, RE::BSGraphics::RendererWindow renderWnd, ID3D11Device* device, ID3D11DeviceContext* context);
		void                            Unregister();
		static Resolution               GetWindowResolution();
		HWND                            GetHwnd() { return wnd; }
		void                            ScaleWindow(Resolution res);
	public:
		Resolution               startingResolution{ 0, 0 };
		WNDPROC                  prevWndProc = 0;
		HWND                     wnd = 0;
		Graphics*                gfx = nullptr;

	private:
		bool                     registered = false;

#ifndef NDEBUG
		WindowsMessageMap        msgMap;
#endif

	};

	
	
}