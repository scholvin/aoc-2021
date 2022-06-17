#include "week2.h"
#include "util.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <iostream>

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

    long day09(char part)
    {
#if 0
        const int WIDTH = 10;
        const int HEIGHT = 5;
        const std::string FILENAME = "../data/day09-smol.dat";
#else
        const int WIDTH = 100;
        const int HEIGHT = 100;
        const std::string FILENAME = "../data/day09.dat";
#endif
        typedef std::pair<uint8_t, uint8_t> coordinate_t;
        std::array<std::array<uint8_t, HEIGHT>, WIDTH> seafloor;
        std::vector<coordinate_t> lows;

        std::ifstream infile(FILENAME);
        std::string line;
        int x = 0, y = 0;
        while (std::getline(infile, line))
        {
            for (x = 0; x < WIDTH; x++)
            {
                seafloor[x][y] = line[x] - '0';
            }
            y++;
        }

        long result = 0;
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                uint8_t left = (x == 0 ? 9 : seafloor[x-1][y]);
                uint8_t right = (x == WIDTH-1 ? 9 : seafloor[x+1][y]);
                uint8_t top = (y == 0 ? 9 : seafloor[x][y-1]);
                uint8_t bottom = (y == HEIGHT-1 ? 9 : seafloor[x][y+1]);
                if (seafloor[x][y] < left && seafloor[x][y] < right && seafloor[x][y] < top && seafloor[x][y] < bottom)
                {
                    if (part == 'a')
                        result += seafloor[x][y] + 1;
                    else
                        lows.push_back(std::make_pair(x, y));
                }
            }
        }

        if (part == 'b')
        {
            // find the "basin" around each low, keep the three biggest ones
            struct coordinate_hash
            {
                std::size_t operator() (const coordinate_t &coordinate) const {
                    return std::hash<uint8_t>()(coordinate.first) ^ std::hash<uint8_t>()(coordinate.second);
                }
            };
            typedef std::unordered_set<coordinate_t, coordinate_hash> basin_t;

            std::multiset<long> areas;
            for (auto l: lows)
            {
                basin_t basin;
                basin.insert(l);
                bool done = false;
                // this is a little bit gross, something like O(b^2) in the size of the basins, but they shouldn't get that big
                while (!done)
                {
                    basin_t work;
                    // for every element in the basin, check all 4 neighbors, and if they're not a 9, and not already in the basin, add them
                    for (auto c: basin)
                    {
                        uint8_t x = c.first;
                        uint8_t y = c.second;
                        // check left
                        if (x > 0 && seafloor[x-1][y] != 9 && basin.find({x-1, y}) == basin.end())
                            work.insert({x-1, y});
                        // right
                        if (x < WIDTH-1 && seafloor[x+1][y] != 9 && basin.find({x+1, y}) == basin.end())
                            work.insert({x+1, y});
                        // top
                        if (y > 0 && seafloor[x][y-1] != 9 && basin.find({x, y-1}) == basin.end())
                            work.insert({x, y-1});
                        // bottom
                        if (y < HEIGHT-1 && seafloor[x][y+1] != 9 && basin.find({x, y+1}) == basin.end())
                            work.insert({x, y+1});
                    }

                    if (work.size() == 0)
                    {
                        done = true;
                    }
                    else
                    {
                        basin.merge(work);
                    }
                }
                areas.insert(basin.size());
            }
            // return product of 3 biggest areas
            result = 1;
            auto it = areas.rbegin();
            result *= *it++;
            result *= *it++;
            result *= *it;

        }

        return result;
    }
};
