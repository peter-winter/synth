#pragma once

#include "generators.hpp"
#include "mix.hpp"
#include "volume.hpp"
#include "synthesizer.hpp"
#include "peak_limiter.hpp"
#include "timeline.hpp"
#include "switch_value.hpp"
#include "note_frequency.hpp"

#include <utility>

struct factory
{
    factory(uint32_t sample_rate):
        sample_rate_(sample_rate)
    {}
    
    template<typename Freq>
    auto sine_wave(Freq freq) { return ::sine_wave<Freq>(sample_rate_, freq); }
    
    template<typename Freq>
    auto square_wave(Freq freq) { return ::square_wave<Freq>(sample_rate_, freq); }
    
    template<typename Freq>
    auto saw_wave(Freq freq) { return ::saw_wave<Freq>(sample_rate_, freq); }

    template<typename Freq>
    auto down_saw_wave(Freq freq) { return [g = ::saw_wave<Freq>(sample_rate_, freq)]() mutable -> float { return -g(); }; }
    
    template<typename Freq>
    auto triangle_wave(Freq freq) { return ::triangle_wave<Freq>(sample_rate_, freq); }
    
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

    template <typename G, typename M, typename A>
    auto mul_add(G g, M m, A a) { return [g, m, a] mutable { return g() * m() + a(); }; }
    
    template<typename T>
    auto bias(T signal) { return [signal] mutable { return signal() * 0.5f + 0.5f; }; }
    
    template <typename... Gs>
    auto synthesizer(uint32_t channel_count, Gs... gs) { return ::synthesizer<Gs...>(sample_rate_, channel_count, gs...); }
    
    uint32_t sample_rate_;
};


