#include "ScreenshotHandler.h"
#include "HRSException.h"
#include "logger.h"



SKSEPluginLoad(const SKSE::LoadInterface *skse) 
{
    SKSE::Init(skse);
    HRS::SetupLog();

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded)
        {
            HRS::ScreenshotHandler::GetSingleton()->Register();
            logger::info("High-Res Screenshots has initialised!");
        }
    });


    return true;
}