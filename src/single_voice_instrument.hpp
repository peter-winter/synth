#pragma once

#include "timeline.hpp"

#include <optional>

template <typename PatchBuilder>
class single_voice_instrument
{
public:
    explicit single_voice_instrument(timeline t, PatchBuilder patch_b)
        : t_(std::move(t)), patch_b_(patch_b), patch_(patch_b_(&t_))
    {}
    
    single_voice_instrument(single_voice_instrument&& other)
        : t_(std::move(other.t_)), patch_b_(other.patch_b_), patch_(patch_b_(&t_))
    {}
    
    single_voice_instrument(const single_voice_instrument&) = delete;
    single_voice_instrument& operator = (const single_voice_instrument&) = delete;

    float operator()()
    {
        t_.prepare_events();
        for (const auto& ev : t_.get_events())
        {
            std::visit(overloaded
            {
                [&](const sound_on&) { patch_.emplace(patch_b_(&t_)); },
                [&](const note_on& on) { patch_.emplace(patch_b_(&t_)); },
                [](auto){}
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
    timeline t_;
    PatchBuilder patch_b_;
    std::optional<to_g_t<PatchBuilder>> patch_;
};
