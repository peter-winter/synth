#pragma once

#include <cmath>

enum class polyphony_scale
{
    equal_amplitude,   // divide-by-N
    equal_power        // divide-by-sqrt(N), supersaw mode
};

constexpr float polyphony_gain(size_t voice_count, polyphony_scale mode)
{
    if (voice_count <= 1) return 1.0f;
    switch (mode)
    {
        case polyphony_scale::equal_power:
            return 1.0f / std::sqrt(static_cast<float>(voice_count));
        case polyphony_scale::equal_amplitude:
            return 1.0f / static_cast<float>(voice_count);
    }
    return 1.0f;
}

template <polyphony_scale Scale = polyphony_scale::equal_amplitude, typename... Gs>
    requires (sizeof...(Gs) > 0)
class mix
{
    static constexpr size_t voice_count = sizeof...(Gs);
    static constexpr float gain = polyphony_gain(voice_count, Scale);

public:
    explicit mix(Gs... g)
        : generators_(g...)
    {}

    float operator()()
    {
        return std::apply([](auto&... gen) { return (gen() + ...) * gain; }, generators_);
    }

private:
    std::tuple<Gs...> generators_;
};
