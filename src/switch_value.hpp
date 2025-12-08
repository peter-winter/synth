#pragma once

#include "timeline.hpp"
#include "utility.hpp"

class switch_value
{
public:
    explicit switch_value(timeline* t)
        : t_(t)
    {}
    
    float operator()()
    {
        for (const auto& ev : t_->get_events())
        {
            std::visit(overloaded{
                [this](const note_on& on) { latched_id_ = on.id; active_ = true; },
                [this](const note_off& off){ if (active_ && off.id == latched_id_) active_ = false; },
                [this](const sound_on&) { active_ = true; },
                [this](const sound_off&) { active_ = false; }
            }, ev.payload);
        }

        return active_ ? 1.0f : 0.0f;
    }
    
private:
    bool active_{false};
    note_id latched_id_{0};
    timeline* t_{nullptr};
};
