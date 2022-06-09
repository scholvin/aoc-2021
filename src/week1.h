#pragma once

#include "runner.h"

namespace week1
{
    long day01a();
    long day01b();
    long day02a();
    long day02b();
    long day03a();
    long day03b();
    long day04(int part);

    const runner::method_map_t method_map =
    {
        { "1a", &day01a },
        { "1b", &day01b },
        { "2a", &day02a },
        { "2b", &day02b },
        { "3a", &day03a },
        { "3b", &day03b },
        { "4a", std::bind(&day04, 1) },
        { "4b", std::bind(&day04, 2) }

    };
};
