#include "week2.h"
#include "util.h"

#include <string>
#include <vector>
//#include <iostream>

namespace week2
{
    struct Sequence
    {
        std::vector<std::string> patterns;
        std::vector<std::string> outputs;
    };

    struct SequenceParser
    {
        Sequence operator()(const std::string& str)
        {
            boost::tokenizer<> tokens(str); // fortunately this gets the spaces and the | in the string
            Sequence ret;
            int i = 0;
            for (auto it = tokens.begin(); it != tokens.end(); it++, i++)
            {
                if (i < 10)
                    ret.patterns.push_back(*it);
                else
                    ret.outputs.push_back(*it);
            }

            return ret;
        }
    };

    long day08a()
    {
        std::vector<Sequence> sequences;
        readers::read_by_line("../data/day08.dat", SequenceParser(), sequences);

        long count = 0;
        for (auto s: sequences)
        {
            for (auto l: s.outputs)
            {
                if (l.size() == 2 || l.size() == 3 || l.size() == 4 || l.size() == 7)
                {
                    count++;
                }
            }
        }

        return count;
    }

    long day08b()
    {
        // I got sick of this badly worded, stupid problem, and cheated with this approach:
        // https://www.reddit.com/r/adventofcode/comments/rc5s3z/2021_day_8_part_2_a_simple_fast_and_deterministic
        // code is mine, I just stole the algorithm
        const std::vector<int> MAGIC = {
            42,
            17,
            34,
            39,
            30,
            37,
            41,
            25,
            49,
            45
        };

        std::vector<Sequence> sequences;
        readers::read_by_line("../data/day08.dat", SequenceParser(), sequences);

        long sum = 0;
        for (auto s: sequences)
        {
            std::map<char, int> decoder = {
                { 'a', 0 },
                { 'b', 0 },
                { 'c', 0 },
                { 'd', 0 },
                { 'e', 0 },
                { 'f', 0 },
                { 'g', 0 },
            };

            for (auto p: s.patterns)
            {
                for (auto c: p)
                    decoder[c]++;
            }

            int mult = 1000;
            for (auto d: s.outputs)
            {
                int digit = 0;
                for (auto c: d)
                {
                    digit += decoder[c];
                }
                auto it = std::find(MAGIC.begin(), MAGIC.end(), digit);
                sum += std::distance(MAGIC.begin(), it) * mult;
                mult /= 10;
            }
        }

        return sum;
    }
};
