#include "Renderer.h"
#include "Window.h"
#include "logger.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "ScreenshotHandler.h"

struct CreateD3DAndSwapChainHook
{
	static void thunk()
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

		ImGui::CreateContext();

		auto& io = ImGui::GetIO();

		io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
		io.IniFilename = nullptr;

		if (!ImGui_ImplWin32_Init(desc.OutputWindow)) {
			logger::error("ImGui initialization failed (Win32)");
			return;
		}

		if (!ImGui_ImplDX11_Init(device, context)) {
			logger::error("ImGui initialization failed (DX11)");
			return;
		}

		HRS::Window::GetSingleton()->Register(desc.OutputWindow, renderer->data.renderWindows[0], device, context);
	}
	static inline REL::Relocation<decltype(thunk)> func;
};

struct DXGIPresentHook
{
	static void thunk(std::uint32_t a_timer)
	{
		func(a_timer);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	static inline REL::Relocation<decltype(thunk)> func;
};


void ImGui::Renderer::Install()
{
	logger::info("Installing ImGui rendering");
	REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(75595, 77226), OFFSET(0x9, 0x275) };  // BSGraphics::InitD3D
	stl::write_thunk_call<CreateD3DAndSwapChainHook>(target.address());

	REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(75461, 77246), 0x9 };               // BSGraphics::Renderer::End
	stl::write_thunk_call<DXGIPresentHook>(target2.address());


	//REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(35556, 36555), OFFSET(0x2D, 0x2D)};
	//stl::write_thunk_call<Main_SetDRS>(target3.address());  
}
