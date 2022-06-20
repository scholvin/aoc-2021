#include "week2.h"
#include "util.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <stack>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>

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
        // cheated here and looked at the data file to avoid dynamic array shenanigans
        const int WIDTH = 100;
        const int HEIGHT = 100;
        const std::string FILENAME = "../data/day09.dat";

        typedef std::pair<uint8_t, uint8_t> coordinate_t;
        std::array<std::array<uint8_t, HEIGHT>, WIDTH> seafloor;
        std::vector<coordinate_t> lows;

        readers::read_dense_2d_matrix(FILENAME, readers::digit_parser(), seafloor);

        long result = 0;
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
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

    long day10a()
    {
        std::vector<std::string> lines;
        readers::read_by_line("../data/day10.dat", lines);

        long sum = 0;
        for (auto line: lines)
        {
            std::stack<char> stack;
            for (auto c: line)
            {
                if (c == '(' || c == '[' || c == '{' || c == '<')
                {
                    stack.push(c);
                }
                else
                {
                    int points = 0;
                    char t = stack.top();
                    stack.pop();
                    if (c == ')' && t != '(')
                        points = 3;
                    else if (c == ']' && t != '[')
                        points = 57;
                    else if (c == '}' && t != '{')
                        points = 1197;
                    else if (c == '>' && t != '<')
                        points = 25137;
                    if (points > 0)
                    {
                        sum += points;
                        break;
                    }
                }
            }
        }

        return sum;
    }

    long day10b()
    {
        std::vector<std::string> lines;
        readers::read_by_line("../data/day10.dat", lines);
        std::vector<long> scores;

        for (auto line: lines)
        {
            std::stack<char> stack;
            bool valid = true;
            for (auto c: line)
            {
                if (c == '(' || c == '[' || c == '{' || c == '<')
                {
                    stack.push(c);
                }
                else
                {
                    char t = stack.top();
                    stack.pop();
                    if ((c == ')' && t != '(') || (c == ']' && t != '[') ||
                        (c == '}' && t != '{') || (c == '>' && t != '<'))
                    {
                        valid = false;
                        break;
                    }
                }
            }
            if (!valid)
            {
                continue;
            }

            std::string completion;
            while (!stack.empty())
            {
                switch (stack.top())
                {
                    case '(': completion += ")"; break;
                    case '[': completion += "]"; break;
                    case '{': completion += "}"; break;
                    case '<': completion += ">"; break;
                }
                stack.pop();
            }
            long score = 0;
            for (auto c: completion)
            {
                score *= 5;
                switch (c)
                {
                    case ')': score += 1; break;
                    case ']': score += 2; break;
                    case '}': score += 3; break;
                    case '>': score += 4; break;
                }
            }
            scores.push_back(score);
        }
        std::sort(scores.begin(), scores.end());
        return scores[(scores.size())/2];
    }

    long day11(char part)
    {
        const int SIZE = 10;
        std::array<std::array<uint8_t, SIZE>, SIZE> grid;
        readers::read_dense_2d_matrix("../data/day11.dat", readers::digit_parser(), grid);

        int steps = 0;
        long flashes = 0;
        long last_flashes = 0;

        while (true)
        {
            std::array<std::array<bool, SIZE>, SIZE> flashed { false };
            // first, increment everyone
            for (int x = 0; x < SIZE; x++)
            {
                for (int y = 0; y < SIZE; y++)
                {
                    grid[x][y]++;
                }
            }
            // iterate on flash algorithm until it settles (no new flashes)
            bool done = false;
            while (!done)
            {
                long prev_flashes = flashes;
                // flash anyone who just turned ten and who hasn't flashed yet
                for (int x = 0; x < SIZE; x++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        if (grid[x][y] > 9 && !flashed[x][y])
                        {
                            flashes++;
                            flashed[x][y] = true;
                            // increment neighbors
                            for (int xn = x-1; xn <= x+1; xn++)
                            {
                                for (int yn = y-1; yn <= y+1; yn++)
                                {
                                    if (xn >= 0 && yn >= 0 && xn < SIZE && yn < SIZE)
                                    {
                                        grid[xn][yn]++;
                                    }
                                }
                            }
                        }
                    }
                }
                done = (flashes == prev_flashes);
            }
            // reset the ones that flashed
            for (int x = 0; x < SIZE; x++)
            {
                for (int y = 0; y < SIZE; y++)
                {
                    if (flashed[x][y])
                    {
                        grid[x][y] = 0;
                    }
                }
            }
            steps++;
            if (part == 'a' && steps == 100)
                return flashes;
            if (part == 'b' && flashes - last_flashes == 100)
                return steps;
            last_flashes = flashes;
        }
    }

    struct vertex_prop_t { std::string name; };
    using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_prop_t>;
    using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
    const std::string START = "start";
    const std::string END = "end";

    // recursive traveler (DFS-ish) for day12a
    long travel_a(const graph_t& graph, const vertex_t present, std::list<vertex_t> small_caves_seen)
    {
        long count = 0;
        if (graph[present].name == END)
        {
            return 1;
        }
        if (graph[present].name[0] >= 'a' && graph[present].name[0] <= 'z')
        {
            small_caves_seen.push_back(present);
        }
        auto ep = boost::out_edges(present, graph);
        for (auto edge_it = ep.first; edge_it != ep.second; edge_it++)
        {
            auto dest = boost::target(*edge_it, graph);
            if (std::find(small_caves_seen.begin(), small_caves_seen.end(), dest) == small_caves_seen.end())
            {
                count += travel_a(graph, dest, small_caves_seen);
            }
        }
        return count;
    }

    // recursive traveler (DFS-ish) for day12b
    long travel_b(const graph_t& graph, const vertex_t present, std::list<vertex_t> small_caves_seen, std::list<vertex_t> small_caves_seen_twice)
    {
        long count = 0;
        if (graph[present].name == END)
        {
            return 1;
        }
        if (graph[present].name[0] >= 'a' && graph[present].name[0] <= 'z')
        {
            if (std::find(small_caves_seen.begin(), small_caves_seen.end(), present) != small_caves_seen.end())
            {
                small_caves_seen_twice.push_back(present);
            }
            else
            {
                small_caves_seen.push_back(present);
            }
        }
        auto ep = boost::out_edges(present, graph);
        for (auto edge_it = ep.first; edge_it != ep.second; edge_it++)
        {
            auto dest = boost::target(*edge_it, graph);
            if (graph[dest].name == START)
            {
                // can't use the start node twice
                continue;
            }
            bool in_small = std::find(small_caves_seen.begin(), small_caves_seen.end(), dest) != small_caves_seen.end();
            if (!in_small || (in_small && small_caves_seen_twice.size() == 0))
            {
                count += travel_b(graph, dest, small_caves_seen, small_caves_seen_twice);
            }
        }
        return count;
    }

    // got some help here: https://www.jasoncoelho.com/2021/12/passage-pathing-advent-of-code-2021-day.html
    long day12(char part)
    {
         // this will never not suck, the need to manage your own map of names to vertices when building a boost::graph
        std::map<std::string, vertex_t> vmap;
        graph_t graph;

        std::ifstream infile("../data/day12.dat");
        std::string line;
        while (std::getline(infile, line))
        {
            std::vector<std::string> vertices;
            boost::algorithm::split(vertices, line, boost::is_any_of("-"));

            auto v = vmap.find(vertices[0]);
            if (v == vmap.end())
            {
                auto v0 = boost::add_vertex(vertex_prop_t { vertices[0]}, graph);
                vmap[vertices[0]] = v0;
            }
            v = vmap.find(vertices[1]);
            if (v == vmap.end())
            {
                auto v1 = boost::add_vertex(vertex_prop_t { vertices[1]}, graph);
                vmap[vertices[1]] = v1;
            }
            boost::add_edge(vmap[vertices[0]], vmap[vertices[1]], graph);
        }

        vertex_t start = vmap.find(START)->second;
        std::list<vertex_t> small_caves_seen;

        if (part == 'a')
        {
            return travel_a(graph, start, small_caves_seen);
        }
        else
        {
            std::list<vertex_t> small_caves_seen_twice;
            return travel_b(graph, start, small_caves_seen, small_caves_seen_twice);
        }
    }

    long day13(char part)
    {
        const std::string FILENAME = "../data/day13.dat";
        // cheats, based on looking at the file
        const int WIDTH = 1311;
        const int HEIGHT = 894;

        typedef std::array<std::array<bool, HEIGHT>, WIDTH> grid_t;
        grid_t grid {false};
        std::ifstream infile(FILENAME);
        std::string line;
        while (true)
        {
            std::getline(infile, line);
            if (line.size() == 0)
            {
                break;
            }
            size_t comma = line.find(',');
            int x = std::stol(line.substr(0, comma));
            int y = std::stol(line.substr(comma+1));
            grid[x][y] = true;
        }

        int width = WIDTH;
        int height = HEIGHT;

#ifndef NDEBUG
        auto debug = [&]() {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    std::cout << (grid[x][y] ? '#' : '.');
                }
                std::cout << std::endl;
            }
        };
#endif

        while (true)
        {
            std::getline(infile, line);
            if (infile.eof())
                break;

            grid_t next { false };
            size_t equal = line.find('=');
            int val = std::stol(line.substr(equal+1));
            if (line[equal-1] == 'x')
            {
                // fold left - populate left part first
                for (int x = 0; x < val; x++)
                {
                    for (int y = 0; y < height; y++)
                    {
                        next[x][y] = grid[x][y];
                    }
                }
                // now flip right over left
                int nx = val - 1;
                for (int x = val + 1; x < width; x++, nx--)
                {
                    for (int y = 0; y < height; y++)
                    {
                        next[nx][y] |= grid[x][y];
                    }
                }
                width = val;
            }
            else
            {
                // fold up - populate top part first
                for (int y = 0; y < val; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        next[x][y] = grid[x][y];
                    }
                }
                // now flip bottom to top
                int ny = val - 1;
                for (int y = val + 1; y < height; y++, ny--)
                {
                    for (int x = 0; x < width; x++)
                    {
                        next[x][ny] |= grid[x][y];
                    }
                }
                height = val;
            }
            grid = next;
            if (part == 'a')
            {
                break;
            }
        }

        long count = 0;
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (grid[x][y])
                {
                    count++;
                }
            }
        }
#ifndef NDEBUG
        if (part == 'b')
        {
            debug(); // lame! output has to be read as alpha characters
        }
#endif
        return count;
    }

    long day14(int steps)
    {
        /*
            count pairs
            break the polymer into a series of pairs (map<string, long>)
            NNCB -> NN NC CB
            apply rules
              NN -> NC CN
              NC -> NB CB
              CB -> CH HB
            now we have NC CN NB CB CH HB
        */

        typedef std::map<std::string, long> polymer_t; // counts pairs
        polymer_t polymer;

        // this is how we manage the pair counts
        auto incr_pair = [](const std::string& pair, polymer_t& poly, long count)
        {
            auto it = poly.find(pair);
            if (it == poly.end())
                poly[pair] = count;
            else
                it->second += count;
        };

        std::ifstream infile("../data/day14.dat");
        std::string line;
        std::getline(infile, line);

        // parse into initial pairs
        for (size_t c = 0; c < line.size() - 1; c++)
        {
            incr_pair(line.substr(c, 2), polymer, 1);
        }

        char final = line[line.size()-1];

        std::getline(infile, line); // throw away blank line

        // build the rules map
        std::map<std::string, std::string> rules;
        while (std::getline(infile, line))
        {
            std::vector<std::string> tmp;
            boost::algorithm::split(tmp, line, boost::is_any_of(" ,->"), boost::token_compress_on);
            rules[tmp[0]] = tmp[1];
        }

        // keep a next generation map - probably some cute way to do this in one, but they're not
        // too huge, so a copy doesn't kill us
        for (int i = 0; i < steps; i++)
        {
            polymer_t next;
            for (auto p: polymer)
            {
                std::string left = p.first[0] + rules[p.first];
                std::string right = rules[p.first] + p.first[1];
                incr_pair(left, next, p.second);
                incr_pair(right, next, p.second);
            }
            polymer = next;
        }

        // count the first character in each pair
        std::array<long, 26> counts{ 0 };
        for (auto p: polymer)
        {
            counts[p.first[0] - 'A'] += p.second;
        }
        // one more for the end of the polymer
        counts[final-'A']++;

        // find the largest and smallest (excluding 0)
        long min = std::numeric_limits<long>::max();
        long max = 0;
        for (auto c: counts)
        {
            if (c > max) max = c;
            if (c < min && c > 0) min = c;
        }

        return max - min;
    }

};
