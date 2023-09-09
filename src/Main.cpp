#include "ScreenshotHandler.h"
#include "HRSException.h"
#include "logger.h"



SKSEPluginLoad(const SKSE::LoadInterface *skse) 
{
    SKSE::Init(skse);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded)
        {
            HRS::SetupLog();
            logger::info("High-Res Screenshots has initialised!");
            HRS::Window::GetSingleton()->Init();
            HRS::ScreenshotHandler::GetSingleton()->Register();
        }
    });


    return true;
}