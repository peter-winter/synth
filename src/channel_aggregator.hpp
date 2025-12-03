#pragma once

#include "strong_types.hpp"

#include <array>
#include <tuple>

template <channels Ch, typename... ChannelGeneratorTuples>
    requires (sizeof...(ChannelGeneratorTuples) == Ch.value_ && sizeof...(ChannelGeneratorTuples) != 0)
class channel_aggregator
{
public:
    explicit channel_aggregator(ChannelGeneratorTuples&&... tuples)
        : channels_(std::forward<ChannelGeneratorTuples>(tuples)...)
    {}

    std::array<float, Ch.value_> operator()()
    {
        return mix_channels(std::make_index_sequence<Ch.value_>{});
    }

private:
    template <typename Tuple>
    static float mix_tuple(Tuple& tuple)
    {
        constexpr size_t N = std::tuple_size_v<Tuple>;
        float sum = 0.0f;
        std::apply([&sum](auto&... gen) { ((sum += gen()), ...); }, tuple);
        return sum / static_cast<float>(N);  // equal loudness per generator
    }

    template <size_t... ChIdx>
    std::array<float, Ch.value_> mix_channels(std::index_sequence<ChIdx...>)
    {
        return { mix_tuple(std::get<ChIdx>(channels_))... };
    }

    std::tuple<ChannelGeneratorTuples...> channels_;
};
