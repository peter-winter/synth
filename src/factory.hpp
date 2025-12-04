#pragma once

#include "strong_types.hpp"
#include "generators.hpp"
#include "mix.hpp"
#include "volume.hpp"
#include "synthesizer.hpp"
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

    auto white_noise() { return ::white_noise{}; }
    
    auto pink_noise() { return ::pink_noise{}; }
    
    template <typename... Gs>
    auto mix(Gs&&... gens) { return ::mix<polyphony_scale::equal_amplitude, Gs...>(std::forward<Gs>(gens)...); }

    template <typename... Gs>
    auto unison(Gs&&... g) { return ::mix<polyphony_scale::equal_power, Gs...>(std::forward<Gs>(g)...); }

    template <typename G, typename V>
    auto volume(G&& g, V&& v) { return ::volume(std::forward<G>(g), std::forward<V>(v)); }
    
    template <typename G, typename Ceil = decltype(constant(0.95f)), typename Release = decltype(constant(0.999f))>
    auto peak_limiter(G&& g, Ceil&& c = constant(0.95f), Release&& r = constant(0.999f)) { return ::peak_limiter<G, Ceil, Release>(std::forward<G>(g), std::forward<Ceil>(c), std::forward<Release>(r)); }

    template <typename... Gs>
    auto synthesizer(Gs&&... g) { return ::synthesizer<Ch, Sr, Gs...>(std::forward<Gs>(g)...); }
    
    template <typename G, typename M, typename A>
    auto mul_add(G&& g, M&& m, A&& a) { return [g = std::move(g), m = std::move(m), a = std::move(a)] mutable { return g() * m() + a(); }; }
};


