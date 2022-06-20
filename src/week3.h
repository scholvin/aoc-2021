#pragma once

#include "runner.h"

namespace week3
{
    long day15();

    const runner::method_map_t method_map =
    {
        { "15a", &day15 },
    };
};
