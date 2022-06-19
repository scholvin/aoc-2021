#pragma once

#include "runner.h"

namespace week2
{
    long day08a();
    long day08b();
    long day09(char part);
    long day10a();
    long day10b();
    long day11(char part);
    long day12(char part);
    long day13(char part);
    long day14(int steps);

    const runner::method_map_t method_map =
    {
        { "8a", &day08a },
        { "8b", &day08b },
        { "9a", std::bind(&day09, 'a') },
        { "9b", std::bind(&day09, 'b') },
        { "10a", &day10a },
        { "10b", &day10b },
        { "11a", std::bind(&day11, 'a') },
        { "11b", std::bind(&day11, 'b') },
        { "12a", std::bind(&day12, 'a') },
        { "12b", std::bind(&day12, 'b') },
        { "13a", std::bind(&day13, 'a') },
        { "13b", std::bind(&day13, 'b') },
        { "14a", std::bind(&day14, 10) },
        { "14b", std::bind(&day14, 40) },

    };
};
