#pragma once

#include "strong_types.hpp"

#include <array>
#include <tuple>

template <channels Ch, typename MonoGenerator>
class channel_copier
{
public:
    explicit channel_copier(MonoGenerator&& mono)
        : mono_(std::move(mono))
    {
    }

    std::array<float, Ch.value_> operator()()
    {
        return fill_channels(std::make_index_sequence<Ch.value_>{});
    }

private:
    template <size_t... I>
    std::array<float, Ch.value_> fill_channels(std::index_sequence<I...>)
    {
        return { (I, mono_())... };
    }

    MonoGenerator mono_;
};
