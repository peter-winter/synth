#pragma once

#include "timeline.hpp"

template <typename Patch>
class instrument
{
public:
    template<typename PatchBuilder>
    explicit instrument(timeline t, PatchBuilder patch_b)
        : t_(t), patch_(patch_b(&t_))
    {}

    float operator()()
    {
        t_.inc();
        return patch_();
    }

private:
    timeline t_;
    Patch patch_;
};
