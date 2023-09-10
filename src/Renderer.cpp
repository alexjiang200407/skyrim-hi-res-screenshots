#include "Renderer.h"
#include "Window.h"
#include "logger.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

struct CreateD3DAndSwapChain
{
	static void thunk()
	{
		logger::info("FAr out brussel sprout");

		CreateD3DAndSwapChain::func();

		const auto renderer = RE::BSGraphics::Renderer::GetSingleton();
		const auto swapChain = renderer->data.renderWindows[0].swapChain;

		DXGI_SWAP_CHAIN_DESC desc{};

		if (FAILED(swapChain->GetDesc(std::addressof(desc))))
			logger::info("SHit");

		const auto context = renderer->data.context;
		const auto device = renderer->data.forwarder;

		ImGui::CreateContext();

		auto& io = ImGui::GetIO();

		io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
		io.IniFilename = nullptr;

		if (!ImGui_ImplWin32_Init(desc.OutputWindow)) {
			logger::error("ImGui initialization failed (Win32)");
			return;
		}

		logger::info("FUCK");

		if (!ImGui_ImplDX11_Init(device, context)) {
			logger::error("ImGui initialization failed (DX11)");
			return;
		}

		HRS::Window::GetSingleton()->Register(desc.OutputWindow);
	}

	static inline REL::Relocation<decltype(thunk)> func;
};

void ImGui::Renderer::Install()
{
	logger::info("Installing ImGui rendering");
	REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(75595, 77226), OFFSET(0x9, 0x275) };  // BSGraphics::InitD3D
	stl::write_thunk_call<CreateD3DAndSwapChain>(target.address());
}
