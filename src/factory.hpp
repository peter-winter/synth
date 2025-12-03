#pragma once

#include "strong_types.hpp"
#include "generators.hpp"
#include "channel_copier.hpp"
#include "channel_aggregator.hpp"
#include "mono_adapter.hpp"
#include "volume.hpp"
#include "synthesizer.hpp"
#include "mull_add.hpp"

template <channels Ch, sample_rate Sr>
struct factory
{
    template<typename Freq>
    auto sine_wave(Freq freq) { return ::sine_wave<Sr, Freq>(std::forward<Freq>(freq)); }
    
    template<typename Freq>
    auto square_wave(Freq freq) { return ::square_wave<Sr, Freq>(std::forward<Freq>(freq)); }
    
    template<typename MonoGenerator>
    auto channel_copier(MonoGenerator&& mono) { return ::channel_copier<Ch, MonoGenerator>(std::forward<MonoGenerator>(mono)); }
    
    template <typename... ChannelGeneratorTuples>
        requires (sizeof...(ChannelGeneratorTuples) == Ch.value_ && sizeof...(ChannelGeneratorTuples) != 0)
    auto channel_aggregator(ChannelGeneratorTuples&&... tuples)
    {
        return ::channel_aggregator<Ch, ChannelGeneratorTuples...>(
            std::forward<ChannelGeneratorTuples>(tuples)...
        );
    }
    
    template <typename MonoGenerator>
    auto mono_adapter(MonoGenerator&& gen) { return ::mono_adapter<MonoGenerator>(std::forward<MonoGenerator>(gen)); }

    template <typename SampleGen, typename VolGen>
    auto volume(SampleGen&& s, VolGen&& v) { return ::volume(std::forward<SampleGen>(s), std::forward<VolGen>(v)); }
    
    template<typename Generator>
    auto synthesizer(Generator&& g, float master_level) { return ::synthesizer<Ch, Sr, Generator>(std::forward<Generator>(g), master_level); }
    
    template <typename Gen, typename MulGen, typename AddGen>
    auto mul_add(Gen&& gen, MulGen&& mul_gen, AddGen&& add_gen) { return ::mul_add<Gen, MulGen, AddGen>(std::forward<Gen>(gen), std::forward<MulGen>(mul_gen), std::forward<AddGen>(add_gen)); }
};

static constexpr auto constant(float v) { return [v]{ return v; }; }
