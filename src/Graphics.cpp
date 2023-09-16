#include "Graphics.h"
#include "logger.h"
#include <DirectXTex.h>
#include "HRS.h"
#include "ScreenshotHandler.h"
#include <sys/stat.h>
#include <iomanip>
#include <ctime>

#pragma comment(lib, "dxgi.lib")

#define GFX_EXCEPT(hr) Graphics::GFxException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define THROW_GFX_EXCEPT(hrcall) { HRESULT hr; infoManager.Set(); if (FAILED(hr = hrcall)) { throw GFX_EXCEPT(hr); } }

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
		auto& settings = ScreenshotHandler::GetSingleton()->settings;
		std::string folder = settings.GetScreenshotFolder();		

		std::wstringstream ss;

		ss << folder.c_str();

		if (*std::prev(std::end(ss.str()), 2) != L'\\')
		{
			ss << L'\\';
		}

		std::wstring wstr = ss.str();
		std::string  path;

		std::transform(wstr.begin(), wstr.end(), std::back_inserter(path), [](wchar_t c) {
			return (char)c;
		});

		logger::info("Screenshot location {}", path);

		struct stat st;
		if (stat(path.c_str(), &st) != 0 || !(st.st_mode & S_IFDIR))
		{
			logger::warn("Can't access folder {} defaulting to .\\", path);
			ss.str(std::wstring(L".\\"));
		}

		ss << RE::PlayerCharacter::GetSingleton()->GetName() << " ";

		{
			auto t = std::time(nullptr);
			auto tm = *std::localtime(&t);

			char buffer[256];
			std::strftime(buffer, 256, "%d-%m-%Y %H-%M-%S", &tm);

			ss << buffer;
		}

		enum DirectX::WICCodecs codec;

		using Type = ScreenshotHandler::ImageType;

		switch (settings.GetImageType())
		{
		case Type::PNG:
		{
			ss << ".png";
			codec = DirectX::WIC_CODEC_PNG;
			break;
		}
		case Type::BMP:
		{
			ss << ".bmp";
			codec = DirectX::WIC_CODEC_BMP;
			break;
		}
		case Type::TIFF:
		{
			ss << ".tiff";
			codec = DirectX::WIC_CODEC_TIFF;
			break;
		}
		case Type::JPG:
		default:
		{
			ss << ".jpg";
			codec = DirectX::WIC_CODEC_JPEG;
			break;
		}
		}

		DirectX::ScratchImage screenshot;
		THROW_GFX_EXCEPT(DirectX::CaptureTexture(device, context, texture, screenshot));
		THROW_GFX_EXCEPT(
			DirectX::SaveToWICFile(
				*screenshot.GetImage(0, 0, 0), 
				DirectX::WIC_FLAGS_FORCE_SRGB & DirectX::WIC_FLAGS_FILTER_CUBIC, 
				DirectX::GetWICCodec(codec),
				ss.str().c_str()
			)
		);
	}
	catch (HRSException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
}

void HRS::Graphics::ResizeTarget(Resolution res)
{
	logger::info("Resizing swapchain target!");

	ID3D11RenderTargetView* rtv;
	context->OMGetRenderTargets(1, &rtv, nullptr);

	rtv->Release();

	try
	{
		DXGI_SWAP_CHAIN_DESC desc;
		THROW_GFX_EXCEPT(swapchain->GetDesc(&desc));
		desc.BufferDesc.Width = res.width;
		desc.BufferDesc.Height = res.height;

		THROW_GFX_EXCEPT(swapchain->ResizeTarget(&desc.BufferDesc));

		logger::info("Finished resizing swapchain target!");
	}
	catch (HRSException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
}

void HRS::Graphics::ResizeBuffer(Resolution res)
{
	logger::info("Resizing buffer to {} {}", res.width, res.height);

	try
	{
		DXGI_SWAP_CHAIN_DESC desc;
		THROW_GFX_EXCEPT(swapchain->GetDesc(&desc));
		context->ClearState();

		THROW_GFX_EXCEPT(swapchain->ResizeBuffers(
			0, res.width, res.height,
			DXGI_FORMAT_UNKNOWN, desc.Flags
		));
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

void HRS::Graphics::CreateD3DAndSwapChainHook::thunk()
{
	func();

	const auto renderer = RE::BSGraphics::Renderer::GetSingleton();
	const auto swapChain = renderer->data.renderWindows[0].swapChain;

	DXGI_SWAP_CHAIN_DESC desc{};

	if (FAILED(swapChain->GetDesc(std::addressof(desc))))
	{
		logger::error("Could not get desc");
		exit(1);
	}

	const auto context = renderer->data.context;
	const auto device = renderer->data.forwarder;

	HRS::Window::GetSingleton()->Register(desc.OutputWindow, renderer->data.renderWindows[0], device, context);
}

void HRS::Graphics::CreateD3DAndSwapChainHook::Install()
{
	REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(75595, 77226), OFFSET(0x9, 0x275) };  // BSGraphics::InitD3D
	stl::write_thunk_call<CreateD3DAndSwapChainHook>(target.address());
}
