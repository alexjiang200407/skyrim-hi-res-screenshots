#include "logger.h"

bool HRS::SetupLog()
{
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder)
    {
        stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
        return false;
    }

    auto logFilePath = *logsFolder / std::format("{}.log", "HiResScreenshots");
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);


    return true;
}
