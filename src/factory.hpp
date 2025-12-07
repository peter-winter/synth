#pragma once

#include "generators.hpp"
#include "mix.hpp"
#include "volume.hpp"
#include "synthesizer.hpp"
#include "peak_limiter.hpp"
#include "timeline.hpp"
#include "switch_value.hpp"
#include "note_frequency.hpp"
#include "single_voice_instrument.hpp"
#include "utility.hpp"

#include <utility>
#include <type_traits>

struct factory
{
    factory(uint32_t sample_rate):
        sample_rate_(sample_rate)
    {}
        
    template<typename F>
    auto sine_wave(F f_b) { return [this, f_b](auto t){ return ::sine_wave<to_g_t<F>>(sample_rate_, f_b(t)); }; }
    
    template<typename F>
    auto square_wave(F f_b) { return [this, f_b](auto t){ return ::square_wave<to_g_t<F>>(sample_rate_, f_b(t)); }; }
    
    template<typename F>
    auto saw_wave(F f_b) { return [this, f_b](auto t){ return ::saw_wave<to_g_t<F>>(sample_rate_, f_b(t)); }; }

    template<typename F>
    auto down_saw_wave(F f_b) { return [this, f_b](auto t)
        { return [g = ::saw_wave<to_g_t<F>>(sample_rate_, f_b(t))]() mutable -> float { return -g(); }; }; }
    
    template<typename F>
    auto triangle_wave(F f_b) { return [this, f_b](auto t){ return ::triangle_wave<to_g_t<F>>(sample_rate_, f_b(t)); }; }
    
    auto white_noise() { return [](auto){ return ::white_noise{}; }; }
    
    auto pink_noise()  { return [](auto){ return ::pink_noise{}; }; }

    template <typename... Gs>
    auto mix(Gs... gs_b) { return [this, gs_b...](auto t){ return ::mix<polyphony_scale::equal_amplitude, to_g_t<Gs>...>(gs_b(t)...); }; }

    template <typename... Gs>
    auto unison(Gs... gs_b) { return [this, gs_b...](auto t){ return ::mix<polyphony_scale::equal_power, to_g_t<Gs>...>(gs_b(t)...); }; }

    template <typename G, typename V>
    auto volume(G g_b, V v_b) { return [this, g_b, v_b](auto t){ return ::volume(g_b(t), v_b(t)); }; }

    template <typename G, typename C = decltype(constant(0.95f)), typename R = decltype(constant(0.999f))>
    auto peak_limiter(G g_b, C c_b = constant(0.95f), R r_b = constant(0.999f)) { return [this, g_b, c_b, r_b](auto t)
        { return ::peak_limiter<to_g_t<G>, to_g_t<C>, to_g_t<R>>(g_b(t), c_b(t), r_b(t)); }; }

    template <typename G, typename M, typename A>
    auto mul_add(G g_b, M m_b, A a_b) { return [this, g_b, m_b, a_b](auto t)
        { return [g = g_b(t), m = m_b(t), a = a_b(t)] mutable { return g() * m() + a(); }; }; }
    
    template<typename T>
    auto bias(T s_b) { return [this, s_b](auto t){ return [s = s_b(t)] mutable { return s() * 0.5f + 0.5f; }; }; }
    
    auto switch_value() { return [](auto t){ return ::switch_value(t); }; }
    
    auto note_frequency() { return [](auto t){ return ::note_frequency(t); }; }
    
    template <typename... Gs>
    auto synthesizer(uint32_t channel_count, Gs... gs) { return ::synthesizer<Gs...>(sample_rate_, channel_count, std::move(gs)...); }
    
    template<typename Pb>
    auto single_voice_instrument(timeline t, Pb p_b) { return ::single_voice_instrument<Pb>(std::move(t), p_b); }
    
    uint32_t sample_rate_;
};


