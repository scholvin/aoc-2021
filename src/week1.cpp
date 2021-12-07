#include "week1.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <list>
#include <cassert>

#if 0

#include <regex>
#include <algorithm>
#include <utility>
#include <functional>
#include <map>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>

#endif

namespace week1
{
    long day01a()
    {
        std::vector<long> input;
        std::ifstream infile("../data/day01.dat");
        std::string line;
        while (std::getline(infile, line))
        {
            input.push_back(std::stol(line));
        }

        long incs = 0;

        for (size_t i = 0; i < input.size(); i++)
        {
            if (i > 0 && input[i] > input[i-1])
                incs++;
        }

        return incs;
    }

    long day01b()
    {
        std::vector<long> input;
        std::ifstream infile("../data/day01.dat");
        std::string line;
        while (std::getline(infile, line))
        {
            input.push_back(std::stol(line));
        }

        long incs = 0;
        long slider = 0;
        const size_t WIN = 3;

        for (size_t i = 0; i < WIN; i++)
        {
            slider += input[i];
        }

        for (size_t i = WIN; i < input.size(); i++)
        {
            long prev = slider;
            slider -= input[i-WIN];
            slider += input[i];

            if (slider > prev)
            {
                incs++;
            }
        }
        return incs;
    }

    long day02a()
    {
        std::vector<std::pair<std::string, long>> input;
        std::ifstream infile("../data/day02.dat");
        std::string line;
        while (std::getline(infile, line))
        {
            size_t space = line.find(" ");
            input.push_back(std::make_pair(line.substr(0, space), std::stol(line.substr(space+1, std::string::npos))));
        }

        long h = 0;
        long d = 0;

        for (auto i: input)
        {
            if (i.first == "forward") h += i.second;
            else if (i.first == "down") d += i.second;
            else if (i.first == "up") d -= i.second;
        }

        return h * d;
    }

    long day02b()
    {
        std::vector<std::pair<std::string, long>> input;
        std::ifstream infile("../data/day02.dat");
        std::string line;
        while (std::getline(infile, line))
        {
            size_t space = line.find(" ");
            input.push_back(std::make_pair(line.substr(0, space), std::stol(line.substr(space+1, std::string::npos))));
        }

        long h = 0;
        long d = 0;
        long aim = 0;
        for (auto i: input)
        {
            if (i.first == "forward")
            {
                h += i.second;
                d += aim * i.second;
            }
            else if (i.first == "down") aim += i.second;
            else if (i.first == "up") aim -= i.second;
        }

        return h * d;
    }

    long day03a()
    {
        std::vector<std::string> input;
        std::ifstream infile("../data/day03.dat");
        std::string line;
        while (std::getline(infile, line))
        {
            input.push_back(line);
        }

        std::string gamma, epsilon;
        for (size_t i = 0; i < input[0].size(); i++)
        {
            size_t ones = 0;
            for (size_t j = 0; j < input.size(); j++)
            {
                if (input[j][i] == '1') ones++;
            }
            gamma += (ones > input.size() / 2) ? "1" : "0";
            epsilon += (ones < input.size() / 2) ? "1" : "0";
        }

        return std::stoull(gamma, 0, 2) * std::stoull(epsilon, 0, 2);
    }

    long day03b()
    {
        std::list<std::string> input;
        std::ifstream infile("../data/day03.dat");
        std::string line;
        while (std::getline(infile, line))
        {
            input.push_back(line);
        }

        auto oxygen = input;
        for (size_t i = 0; i < input.begin()->size(); i++)
        {
            size_t ones = 0;
            for (auto it: oxygen)
            {
                if ((it)[i] == '1') ones++;
            }
            char keep = '0';
            if (ones >= oxygen.size() - ones) keep = '1';
            for (auto it = oxygen.begin(); it != oxygen.end(); )
            {
                if ((*it)[i] == keep)
                {
                    ++it;
                }
                else
                {
                    it = oxygen.erase(it);
                }
                if (oxygen.size() == 1)
                    break;
            }
            if (oxygen.size() == 1)
                    break;
        }
        assert(oxygen.size() == 1);

        auto co2 = input;
        for (size_t i = 0; i < input.begin()->size(); i++)
        {
            size_t ones = 0;
            for (auto it: co2)
            {
                if ((it)[i] == '1') ones++;
            }
            char keep = '0';
            if (ones < co2.size() - ones) keep = '1';
            for (auto it = co2.begin(); it != co2.end(); )
            {
                if ((*it)[i] == keep)
                {
                    ++it;
                }
                else
                {
                    it = co2.erase(it);
                }
                if (co2.size() == 1)
                    break;
            }
            if (co2.size() == 1)
                break;

        }
        assert(co2.size() == 1);

        return std::stoull(oxygen.front(), 0, 2) * std::stoull(co2.front(), 0, 2);
    }

};
