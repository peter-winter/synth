#pragma once

constexpr inline auto constant(float v)
{
    return [v]{ return v; };
}
