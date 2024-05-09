#include "Settings.h"
#include "Utility.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\DynamicWaitSKSE.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    wait_hotkey                     = ini.GetLongValue("General", "uWaitHotkey");
    wait_multiplier                 = static_cast<float>(ini.GetDoubleValue("General", "fWaitMultiplier"));
    Utility::multiplier_accumulator = wait_multiplier;
    ramp_up_multiplier              = ini.GetBoolValue("General", "bRampUpMultiplier");
    clamp_multiplier                = static_cast<float>(ini.GetDoubleValue("General", "fClampMultiplier"));

    logger::info("Loaded settings");
    logger::info("\tuWaitHotkey = {}", wait_hotkey);
    logger::info("\tfWaitMultiplier = {}", wait_multiplier);
    logger::info("\tbRampUpMultiplier = {}", ramp_up_multiplier);
    logger::info("\tfClampMultiplier = {}", clamp_multiplier);
    logger::info("");
}
