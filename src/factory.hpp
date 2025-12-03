#pragma once

#include "strong_types.hpp"
#include "generators.hpp"
#include "mix.hpp"
#include "volume.hpp"
#include "synthesizer.hpp"
#include "mull_add.hpp"
#include "peak_limiter.hpp"

template <channels Ch, sample_rate Sr>
struct factory
{
    template<typename Freq>
    auto sine_wave(Freq freq) { return ::sine_wave<Sr, Freq>(std::forward<Freq>(freq)); }
    
    template<typename Freq>
    auto square_wave(Freq freq) { return ::square_wave<Sr, Freq>(std::forward<Freq>(freq)); }
    
    template<typename Freq>
    auto saw_wave(Freq freq) { return ::saw_wave<Sr, Freq>(std::forward<Freq>(freq)); }

    template <typename... Generators>
    auto mix(Generators&&... gens) { return ::mix<polyphony_scale::equal_amplitude, Generators...>(std::forward<Generators>(gens)...); }

    template <typename... Generators>
    auto unison(Generators&&... gens) { return ::mix<polyphony_scale::equal_power, Generators...>(std::forward<Generators>(gens)...); }

    template <typename SampleGen, typename VolGen>
    auto volume(SampleGen&& s, VolGen&& v) { return ::volume(std::forward<SampleGen>(s), std::forward<VolGen>(v)); }
    
    template <typename InputGen, typename CeilingGen = decltype(constant(0.95f)), typename ReleaseGen = decltype(constant(0.999f))>
    auto peak_limiter(InputGen&& in, CeilingGen&& ceiling = constant(0.95f), ReleaseGen&& release = constant(0.999f)) { return ::peak_limiter<InputGen, CeilingGen, ReleaseGen>(std::forward<InputGen>(in), std::forward<CeilingGen>(ceiling), std::forward<ReleaseGen>(release)); }

    template <typename... MonoGenerators>
    auto synthesizer(MonoGenerators&&... gens) { return ::synthesizer<Ch, Sr, MonoGenerators...>(std::forward<MonoGenerators>(gens)...); }
    
    template <typename Gen, typename MulGen, typename AddGen>
    auto mul_add(Gen&& gen, MulGen&& mul_gen, AddGen&& add_gen) { return ::mul_add<Gen, MulGen, AddGen>(std::forward<Gen>(gen), std::forward<MulGen>(mul_gen), std::forward<AddGen>(add_gen)); }
};


