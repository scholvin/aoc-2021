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
    long day04(char part);
    long day05(char part);
    long day06(int turns);

    const runner::method_map_t method_map =
    {
        { "1a", &day01a },
        { "1b", &day01b },
        { "2a", &day02a },
        { "2b", &day02b },
        { "3a", &day03a },
        { "3b", &day03b },
        { "4a", std::bind(&day04, 'a') },
        { "4b", std::bind(&day04, 'b') },
        { "5a", std::bind(&day05, 'a') },
        { "5b", std::bind(&day05, 'b') },
        { "6a", std::bind(&day06, 80) },
        { "6b", std::bind(&day06, 256) },
    };
};
