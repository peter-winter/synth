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
    auto sine_wave(Freq freq) { return ::sine_wave<Sr, Freq>(freq); }
    
    template<typename Freq>
    auto square_wave(Freq freq) { return ::square_wave<Sr, Freq>(freq); }
    
    template<typename Freq>
    auto saw_wave(Freq freq) { return ::saw_wave<Sr, Freq>(freq); }

    template<typename Freq>
    auto down_saw_wave(Freq freq) { return [g = saw_wave(freq)]() mutable -> float { return -g(); }; }
    
    template<typename Freq>
    auto triangle_wave(Freq freq) { return ::triangle_wave<Sr, Freq>(freq); }
    
    auto white_noise() { return ::white_noise{}; }
    
    auto pink_noise()  { return ::pink_noise{}; }

    template <typename... Gs>
    auto mix(Gs... gs) { return ::mix<polyphony_scale::equal_amplitude, Gs...>(gs...); }

    template <typename... Gs>
    auto unison(Gs... gs) { return ::mix<polyphony_scale::equal_power, Gs...>(gs...); }

    template <typename G, typename V>
    auto volume(G g, V v) { return ::volume(g, v); }

    template <typename G, typename C = decltype(constant(0.95f)), typename R = decltype(constant(0.999f))>
    auto peak_limiter(G g, C c = constant(0.95f), R r = constant(0.999f)) { return ::peak_limiter<G, C, R>(g, c, r); }

    template <typename... Gs>
    auto synthesizer(Gs... gs) { return ::synthesizer<Ch, Sr, Gs...>(gs...); }

    template <typename G, typename M, typename A>
    auto mul_add(G g, M m, A a) { return [g, m, a] mutable { return g() * m() + a(); }; }
    
    template<typename T>
    auto bias(T signal) { return [signal] mutable { return signal() * 0.5f + 0.5f; }; }
};


