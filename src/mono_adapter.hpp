#pragma once

#include <array>
#include "strong_types.hpp"

template <typename MonoGenerator>
class mono_adapter
{
public:
    explicit mono_adapter(MonoGenerator&& gen)
        : gen_(std::move(gen))
    {}

    std::array<float, 1> operator()()
    {
        return { gen_() };
    }

private:
    MonoGenerator gen_;
};
