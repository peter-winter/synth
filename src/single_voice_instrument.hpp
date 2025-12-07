#pragma once

#include "timeline.hpp"

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

    float operator()()
    {
        t_.inc();
        
        auto events = t_.get_events();
        for (const auto& ev : events)
        {
            std::visit(overloaded
            {
                [this](const note_on& on) { patch_ = patch_b_(&t_); },
                [this](const note_off& off) {}
            }, ev.payload);
        }
        
        return patch_();
    }

private:
    timeline t_;
    PatchBuilder patch_b_;
    to_g_t<PatchBuilder> patch_;
};
