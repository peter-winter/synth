#pragma once

#include "timeline.hpp"
#include "voice.hpp"

#include <optional>

template <typename PatchBuilder>
class mono_instrument
{
public:
    explicit mono_instrument(timeline t, PatchBuilder patch_b)
        : t_(std::move(t)), patch_b_(patch_b)
    {}
    
    mono_instrument(mono_instrument&& other) = default;
    
    mono_instrument(const mono_instrument&) = delete;
    mono_instrument& operator = (const mono_instrument&) = delete;

    float operator()()
    {
        t_.prepare_events();
        for (const auto& ev : t_.get_events())
        {
            std::visit(overloaded
            {
                [&](const sound_on& on) { do_on(on.id_); },
                [&](const note_on& on) { do_on(on.id_, on.f_); },
                [&](const sound_off& off) { do_off(off.id_); },
                [&](const note_off& off) { do_off(off.id_); }
            }, ev.payload);
        }
        
        float sample = patch_.has_value() ? patch_.value()() : 0.0f;
        t_.inc();
        return sample;
    }
    
    void reset()
    {
        t_.reset();
        patch_.reset();
    }
    
private:
    void do_on(uint32_t id, std::optional<frequency> f = std::nullopt)
    {
        if (!v_.active_)
            patch_.emplace(patch_b_(&v_));
        v_.active_ = true;
        v_.note_id_ = id;
        v_.f_ = f;
    }
    
    void do_off(uint32_t id)
    {
        if (v_.note_id_ == id)
            v_.active_ = false;   
    }
    
    timeline t_;
    PatchBuilder patch_b_;
    std::optional<to_g_t<PatchBuilder>> patch_;
    voice v_;
};
