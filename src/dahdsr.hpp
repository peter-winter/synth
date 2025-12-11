#pragma once

#include "voice.hpp"
#include "constant.hpp"
#include <chrono>
#include <algorithm>
#include <cmath>

enum class dahdsr_stage : uint8_t
{
    off,
    delay,
    attack,
    hold,
    decay,
    sustain,
    release
};

template<typename DelayGen, typename AttackGen, typename HoldGen,
         typename DecayGen, typename SustainGen, typename ReleaseGen>
class dahdsr
{
public:
    static constexpr float min_attack_sec  = 0.008f;
    static constexpr float min_release_sec = 0.008f;
    static constexpr float min_decay_sec   = 0.008f;

private:
    inline uint64_t time_to_samples(float time_sec) const
    {
        return static_cast<uint64_t>(time_sec * sample_rate_ + 0.5f);
    }

    template<typename T>
    inline float to_float_time(T t)
    {
        return std::chrono::duration_cast<std::chrono::duration<float>>(t).count();
    }
    
    void capture_times()
    {
        delay_sec_ = std::max(0.0f, to_float_time(delay_gen_()));
        attack_sec_ = std::max(min_attack_sec, to_float_time(attack_gen_()));
        hold_sec_ = std::max(0.0f, to_float_time(hold_gen_()));
        release_sec_ = std::max(min_release_sec, to_float_time(release_gen_()));

        sustain_target_ = sustain_gen_();

        decay_sec_ = (sustain_target_ != 1.0f)
            ? std::max(min_decay_sec, to_float_time(decay_gen_()))
            : std::max(0.0f, to_float_time(decay_gen_()));
        
        decay_dir_ = sustain_target_ >= 1.0 ? up_dir : down_dir;
    }

    template<typename F>
    float exp_segment(float start, float target, float time_sec, dahdsr_stage next_stage, F f)
    {
        if (time_sec <= 0.0f)
        {
            stage_ = next_stage;
            return target;
        }

        if (time_ >= time_sec)
        {
            stage_ = next_stage;
            time_ -= time_sec;
            return target;
        }

        float p = time_ / time_sec;
        return f(p);
    }
    
    float up_exp_segment(float start, float target, float time_sec, dahdsr_stage next_stage)
    {
        return exp_segment(start, target, time_sec, next_stage, up_dir);
    }
    
    float down_exp_segment(float start, float target, float time_sec, dahdsr_stage next_stage)
    {
        return exp_segment(start, target, time_sec, next_stage, down_dir);
    }
    
    static float up_dir(float x){ return 1.0f - std::pow(1.0f - x, 4.5f); }
    static float down_dir(float x){ return std::pow(1.0f - x, 4.5f); }

public:
    dahdsr(voice* v, uint32_t sample_rate,
           DelayGen   delay_gen,
           AttackGen  attack_gen,
           HoldGen    hold_gen,
           DecayGen   decay_gen,
           SustainGen sustain_gen,
           ReleaseGen release_gen)
        : v_(v),
        sample_rate_(sample_rate),
        delay_gen_(std::move(delay_gen)),
        attack_gen_(std::move(attack_gen)),
        hold_gen_(std::move(hold_gen)),
        decay_gen_(std::move(decay_gen)),
        sustain_gen_(std::move(sustain_gen)),
        release_gen_(std::move(release_gen)),
        time_(0.0f)
    {
    }

    float operator()()
    {
        float out = 0.0f;
        
        switch (stage_)
        {
            case dahdsr_stage::off:
            {
                if (v_->active_)
                {
                    capture_times();
                    stage_ = delay_sec_ > 0.0f ? dahdsr_stage::delay : dahdsr_stage::attack;
                }
                break;
            }

            case dahdsr_stage::delay:
            {
                if (time_ >= delay_sec_)
                {
                    stage_ = dahdsr_stage::attack;
                    time_ -= delay_sec_;
                }
                break;
            }

            case dahdsr_stage::attack:
            {
                out = up_exp_segment(0.0f, 1.0f, attack_sec_, hold_sec_ > 0.0f ? dahdsr_stage::hold : dahdsr_stage::decay);
                break;
            }

            case dahdsr_stage::hold:
            {
                out = 1.0f;
                if (time_ >= hold_sec_)
                {
                    stage_ = dahdsr_stage::decay;
                    time_ -= hold_sec_;
                }
                break;
            }

            case dahdsr_stage::decay:
            {
                out = exp_segment(1.0f, sustain_target_, decay_sec_, dahdsr_stage::sustain, decay_dir_);
                break;
            }

            case dahdsr_stage::sustain:
            {
                time_ = 0.0f;
                out = sustain_gen_();

                if (!v_->active_)
                {
                    sustain_at_release_ = out;
                    stage_ = dahdsr_stage::release;
                }
                break;
            }

            case dahdsr_stage::release:
            {
                if (v_->active_)
                {
                    capture_times();
                    stage_ = (delay_sec_ > 0.0f ? dahdsr_stage::delay : dahdsr_stage::attack);
                    break;
                }

                out = down_exp_segment(sustain_at_release_, 0.0f, release_sec_, dahdsr_stage::off);
                break;
            }
        }

        time_ += 1.0f / sample_rate_;
        
        if (out < 0.0)
            return 0.0f;
        return out;
    }

private:
    voice* v_{nullptr};
    uint32_t sample_rate_;

    DelayGen   delay_gen_;
    AttackGen  attack_gen_;
    HoldGen    hold_gen_;
    DecayGen   decay_gen_;
    SustainGen sustain_gen_;
    ReleaseGen release_gen_;

    float delay_sec_{0.0f};
    float attack_sec_{min_attack_sec};
    float hold_sec_{0.0f};
    float decay_sec_{min_decay_sec};
    float release_sec_{min_release_sec};

    dahdsr_stage stage_{dahdsr_stage::off};
    uint64_t     counter_{0};
    float        sustain_at_release_{0.0f};
    float        sustain_target_{1.0f};
    
    float (*decay_dir_)(float);
    float time_{0.0f};
};
