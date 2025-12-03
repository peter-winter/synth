#pragma once

#include "math.hpp"
#include <tuple>
#include <utility>

template <polyphony_scale Scale = polyphony_scale::equal_amplitude, typename... Generators>
    requires (sizeof...(Generators) > 0)
class mix
{
    static constexpr size_t voice_count = sizeof...(Generators);
    static constexpr float gain = polyphony_gain_t<voice_count, Scale>::value;

public:
    explicit mix(Generators&&... gens)
        : generators_(std::forward<Generators>(gens)...)
    {}

    float operator()()
    {
        return std::apply([](auto&... gen) { return (gen() + ...) * gain; }, generators_);
    }

private:
    std::tuple<Generators...> generators_;
};
