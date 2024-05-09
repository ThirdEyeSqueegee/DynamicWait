#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool debug_logging{};

    inline static std::uint32_t wait_hotkey{};

    inline static float wait_multiplier{};

    inline static bool ramp_up_multiplier{};

    inline static float clamp_multiplier{};
};
