#include "ScreenshotHandler.h"
#include "HRSException.h"



SKSEPluginLoad(const SKSE::LoadInterface *skse) 
{
    SKSE::Init(skse);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded)
        {
            RE::ConsoleLog::GetSingleton()->Print("Hi-Res Screenshots has initiated");
            HRS::ScreenshotHandler::GetSingleton()->Screenshot();
        }
    });


    return true;
}