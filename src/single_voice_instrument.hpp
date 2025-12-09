#pragma once

#include "timeline.hpp"
#include "voice.hpp"

#include <optional>

template <typename PatchBuilder>
class single_voice_instrument
{
public:
    explicit single_voice_instrument(timeline t, PatchBuilder patch_b)
        : t_(std::move(t)), patch_b_(patch_b)
    {}
    
    single_voice_instrument(single_voice_instrument&& other)
        : t_(std::move(other.t_)), patch_b_(other.patch_b_)
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
                [&](const sound_on& on) { do_sound_on(on); },
                [&](const note_on& on) { do_note_on(on); },
                [&](const sound_off& off) { do_sound_off(off); },
                [&](const note_off& off) { do_note_off(off); }
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
    void do_sound_on(const sound_on& on)
    {
        if (!v_.active_)
        {
            v_.active_ = true;
            v_.note_id_ = on.id_;
            patch_.emplace(patch_b_(&v_));
        }
        else
            v_.note_id_ = on.id_;
    }
    
    void do_sound_off(const sound_off& off)
    {
        if (off.id_ == v_.note_id_)
            v_.active_ = false;   
    }
    
    void do_note_on(const note_on& on)
    {
        if (!v_.active_)
        {
            v_.active_ = true;
            v_.note_id_ = on.id_;
            v_.f_ = on.f_;
            patch_.emplace(patch_b_(&v_));
        }
        else
        {
            v_.note_id_ = on.id_;
            v_.f_ = on.f_;;
        }
    }
    
    void do_note_off(const note_off& off)
    {
        if (off.id_ == v_.note_id_)
            v_.active_ = false;
    }
    
    timeline t_;
    PatchBuilder patch_b_;
    std::optional<to_g_t<PatchBuilder>> patch_;
    voice v_;
};
