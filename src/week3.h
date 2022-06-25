#pragma once

#include "runner.h"

namespace week3
{
    long day15a();
    long day15b();
    long day16(char part);

    const runner::method_map_t method_map =
    {
        { "15a", &day15a },
        { "15b", &day15b },
        { "16a", std::bind(&day16, 'a') },
        { "16b", std::bind(&day16, 'b') }
    };
};
