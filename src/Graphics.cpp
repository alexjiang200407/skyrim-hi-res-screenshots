#include "Graphics.h"
#include "logger.h"
#include <DirectXTex.h>

#pragma comment(lib, "dxgi.lib")

#define GFX_EXCEPT(hr) Graphics::GFxException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define THROW_GFX_EXCEPT(hrcall) {HRESULT hr; infoManager.Set(); if (FAILED(hr = hrcall)) { throw GFX_EXCEPT(hr); } }

HRS::Graphics::~Graphics()
{
	swapchain->Release();
	device->Release();
	context->Release();
}

void HRS::Graphics::CaptureScreen()
{
	try
	{
		logger::info("Capturing the screen");
		ID3D11Texture2D* texture;

		THROW_GFX_EXCEPT(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture));
		CaptureScreen(texture);
		texture->Release();
	}
	catch (HRSException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
}

void HRS::Graphics::CaptureScreen(ID3D11Texture2D* texture)
{
	try
	{
		DirectX::ScratchImage screenshot;
		THROW_GFX_EXCEPT(DirectX::CaptureTexture(device, context, texture, screenshot));
		THROW_GFX_EXCEPT(DirectX::SaveToWICFile(*screenshot.GetImage(0, 0, 0), DirectX::WIC_FLAGS_FORCE_SRGB, DirectX::GetWICCodec(DirectX::WIC_CODEC_JPEG), L"C:\\Users\\alexj\\OneDrive\\Documents\\My Games\\Screenshots\\poo.jpg"));
	}
	catch (HRSException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
}

const char* HRS::Graphics::GFxException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "Error String: " << GetErrorString() << std::endl
		<< "Description: " << GetErrorDescription() << std::endl;

	if (!info.empty())
	{
		oss << "\nError Info:\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOrigin();
	buffer = oss.str();
	return buffer.c_str();
}

std::string HRS::Graphics::GFxException::GetErrorDescription() const
{
	char desc[512];
	DXGetErrorDescriptionA(hr, desc, 512);
	return desc;
}