#include "HRS.h"
#include "HRSException.h"
#include "logger.h"
#include "Window.h"
#include "ScreenshotHandler.h"

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse)
{
    HRS::SetupLog();
    logger::info("High-Res Screenshots has initialised!");
    SKSE::Init(skse);

    try
    {
        HRS::Graphics::CreateD3DAndSwapChainHook::Install();
    }
    catch (const HRSException& e)
    {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
        exit(1);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "Unknown Exception", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
        exit(1);
    }

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded)
        {
            HRS::HiResScreenshots::GetSingleton()->Register();
        }

    });

    return true;
}