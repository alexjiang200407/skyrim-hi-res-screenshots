#pragma once
#include <d3d11.h>
#include "HRSException.h"
#include "DxgiInfoManager.h"
#include "dxerr.h"
#include "Resolution.h"

namespace HRS
{
	class Graphics
	{
	public:
		class GFxException
			:
			public HRSException
		{
		public:
			GFxException() = delete;
			GFxException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {})
				:
				HRSException(line, file), hr(hr)
			{}

			const char* what() const override;
			const char* GetType() const override { return "Graphics Exception"; }
			HRESULT     GetErrorCode() const { return hr; }
			std::string GetErrorString() const { return DXGetErrorStringA(hr); }
			std::string GetErrorDescription() const;
			std::string GetErrorInfo() const { return info; }

		private:
			HRESULT     hr;
			std::string info;
		};
	public:
		Graphics() = delete;
		Graphics(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapchain)
			:
			hWnd(hWnd), device(device), context(context), swapchain(swapchain)
		{}
		~Graphics();
		Graphics(const Graphics&) = delete;
		Graphics(const Graphics&&) = delete;
		Graphics& operator=(const Graphics&) = delete;


	public:
		void CaptureScreen();
		void CaptureScreen(ID3D11Texture2D* texture);


	public:
		HWND                 hWnd;
		ID3D11Device*        device;
		ID3D11DeviceContext* context;
		IDXGISwapChain*      swapchain;

	private:
		DxgiInfoManager      infoManager;
	};
}
