#pragma once

#include "runner.h"

namespace week2
{
    long day08a();
    long day08b();
    long day09(char part);
    long day10a();
    long day10b();

    const runner::method_map_t method_map =
    {
        { "8a", &day08a },
        { "8b", &day08b },
        { "9a", std::bind(&day09, 'a') },
        { "9b", std::bind(&day09, 'b') },
        { "10a", &day10a },
        { "10b", &day10b },

    };
};
