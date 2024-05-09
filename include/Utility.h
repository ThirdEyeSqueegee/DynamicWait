#pragma once

class Utility : public Singleton<Utility>
{
public:
    inline static bool waiting{};

    inline static float multiplier_accumulator{};
};
