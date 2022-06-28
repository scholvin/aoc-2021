#pragma once

#include "runner.h"

namespace week3
{
    long day15a();
    long day15b();
    long day16a();
    // long day16b(); vexing
    long day17(char part);

    const runner::method_map_t method_map =
    {
        { "15a", &day15a },
        { "15b", &day15b },
        { "16a", &day16a  },
        { "17a", std::bind(&day17, 'a') },
        { "17b", std::bind(&day17, 'b') },
    };
};
