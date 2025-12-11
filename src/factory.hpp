#pragma once

#include "generators.hpp"
#include "mix.hpp"
#include "volume.hpp"
#include "synthesizer.hpp"
#include "peak_limiter.hpp"
#include "timeline.hpp"
#include "dahdsr.hpp"
#include "note_frequency.hpp"
#include "mono_instrument.hpp"
#include "poly_instrument.hpp"
#include "utility.hpp"

#include <utility>
#include <type_traits>

struct factory
{
    factory(uint32_t sample_rate):
        sample_rate_(sample_rate)
    {}
        
    template<typename F>
    auto sine_wave(F f_b) { return [sr = sample_rate_, f_b](auto v){ return ::sine_wave<to_g_t<F>>(sr, f_b(v)); }; }
    
    template<typename F>
    auto square_wave(F f_b) { return [sr = sample_rate_, f_b](auto v){ return ::square_wave<to_g_t<F>>(sr, f_b(v)); }; }
    
    template<typename F>
    auto saw_wave(F f_b) { return [sr = sample_rate_, f_b](auto v){ return ::saw_wave<to_g_t<F>>(sr, f_b(v)); }; }

    template<typename F>
    auto down_saw_wave(F f_b) { return [sr = sample_rate_, f_b](auto v)
        { return [g = ::saw_wave<to_g_t<F>>(sr, f_b(v))]() mutable -> float { return -g(); }; }; }
    
    template<typename F>
    auto triangle_wave(F f_b) { return [sr = sample_rate_, f_b](auto v){ return ::triangle_wave<to_g_t<F>>(sr, f_b(v)); }; }
    
    auto white_noise() { return [](auto){ return ::white_noise{}; }; }
    
    auto pink_noise()  { return [](auto){ return ::pink_noise{}; }; }

    template <typename... Gs>
    auto mix(Gs... gs_b) { return [gs_b...](auto v){ return ::mix<polyphony_scale::equal_amplitude, to_g_t<Gs>...>(gs_b(v)...); }; }

    template <typename... Gs>
    auto unison(Gs... gs_b) { return [gs_b...](auto v){ return ::mix<polyphony_scale::equal_power, to_g_t<Gs>...>(gs_b(v)...); }; }

    template <typename G, typename V>
    auto volume(G g_b, V v_b) { return [g_b, v_b](auto v){ return ::volume(g_b(v), v_b(v)); }; }

    template <typename G, typename C = decltype(constant(0.95f)), typename R = decltype(constant(0.999f))>
    auto peak_limiter(G g_b, C c_b = constant(0.95f), R r_b = constant(0.999f)) { return [g_b, c_b, r_b](auto v)
        { return ::peak_limiter<to_g_t<G>, to_g_t<C>, to_g_t<R>>(g_b(v), c_b(v), r_b(v)); }; }

    template <typename G, typename M, typename A>
    auto mul_add(G g_b, M m_b, A a_b) { return [g_b, m_b, a_b](auto v)
        { return [g = g_b(v), m = m_b(v), a = a_b(v)] mutable { return g() * m() + a(); }; }; }
    
    template <typename G, typename M>
    auto mul(G g_b, M m_b) { return [g_b, m_b](auto v)
        { return [g = g_b(v), m = m_b(v)] mutable { return g() * m(); }; }; }
        
    template<typename T>
    auto bias(T s_b) { return [s_b](auto v){ return [s = s_b(v)] mutable { return s() * 0.5f + 0.5f; }; }; }
    
    template <typename G, typename A>
    auto add(G g_b, A a_b) { return [g_b, a_b](auto v)
        { return [g = g_b(v), a = a_b(v)] mutable { return g() + a(); }; }; }
    
    auto note_frequency() { return [](auto v){ return ::note_frequency(v); }; }
    
    template <typename... Gs>
    auto synthesizer(uint32_t channel_count, Gs... gs) { return ::synthesizer<Gs...>(sample_rate_, channel_count, std::move(gs)...); }
    
    template<typename Pb>
    auto mono_instrument(timeline t, Pb p_b) { return ::mono_instrument<Pb>(std::move(t), p_b); }
    
    template<typename Pb>
    auto poly_instrument(uint32_t max_voices, timeline t, Pb p_b) { return ::poly_instrument<Pb>(max_voices, std::move(t), std::move(p_b)); }
    
    template<typename Pb>
    auto unison_instrument(uint32_t max_voices, timeline t, Pb p_b) { return ::poly_instrument<Pb, polyphony_scale::equal_power>(max_voices, std::move(t), std::move(p_b)); }
    
    template<typename Dly, typename Atk, typename Hld, typename Dec, typename Sus, typename Rel>
    auto env_dahdsr(Dly dly_b, Atk atk_b, Hld hld_b, Dec dec_b, Sus sus_b, Rel rel_b)
    {
        return [sr = sample_rate_, dly_b, atk_b, hld_b, dec_b, sus_b, rel_b](auto v) { return ::dahdsr(v, sr, dly_b(v), atk_b(v), hld_b(v), dec_b(v), sus_b(v), rel_b(v)); };
    }
    
    auto env_sudden() { using namespace std::chrono_literals; return env_dahdsr(constant(0.0s), constant(0.0s), constant(0.0s), constant(0.0s), constant(1.0f), constant(0.0s)); }
    
    template<typename Rel>
    auto env_r(Rel rel_b) { using namespace std::chrono_literals; return env_dahdsr(constant(0.0s), constant(0.0s), constant(0.0s), constant(0.0s), constant(1.0f), rel_b);}
    
    template<typename Atk, typename Rel>
    auto env_ar(Atk atk_b, Rel rel_b) { using namespace std::chrono_literals; return env_dahdsr(constant(0.0s), atk_b, constant(0.0s), constant(0.0s), constant(1.0f), rel_b);}
    
    uint32_t sample_rate_;
};


