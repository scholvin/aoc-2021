#include "week1.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <list>
#include <cassert>

#include <boost/algorithm/string.hpp>

#if 0

#include <regex>
#include <algorithm>
#include <utility>
#include <functional>
#include <map>
#include <set>


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

    class Bingo {
    public:
        Bingo(const std::vector<std::string>& rows)
        {
            for (int i = 0; i < 5; i++)
            {
                std::vector<std::string> parts;
                boost::algorithm::split(parts, rows[i], boost::is_any_of(" "), boost::token_compress_on);
                for (int j = 0; j < 5; j++)
                {
                    m_board[i][j].first = std::stol(parts[j]);
                    m_board[i][j].second = false;
                }
            }
        }

        // true if this play makes a winner
        bool play(int num)
        {
            bool check = false;
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (m_board[i][j].first == num)
                    {
                        m_board[i][j].second = true;
                        check = true;
                        break;
                    }
                }
                if (check) break;
            }

            if (check)
            {
                // check rows and columns
                for (int i = 0; i < 5; i++)
                {
                    if (m_board[i][0].second && m_board[i][1].second && m_board[i][2].second && m_board[i][3].second && m_board[i][4].second)
                        return true;
                    if (m_board[0][i].second && m_board[1][i].second && m_board[2][i].second && m_board[3][i].second && m_board[4][i].second)
                        return true;
                }
            }
            return false;
        }

        // return the score, which is the sum of all unmarked cells times the number just played
        long score(int num) const
        {
            long sum = 0;
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (!m_board[i][j].second)
                    {
                        sum += m_board[i][j].first;
                    }
                }
            }
            return sum * num;
        }

        void print() const
        {
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    std::cout << m_board[i][j].first << (m_board[i][j].second ? "." : "") << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    private:
        typedef std::pair<int, bool> Cell;
        Cell m_board[5][5];
    };

    long day04(int part)
    {
        std::ifstream infile("../data/day04.dat");
        std::string draw;
        std::getline(infile, draw);
        std::vector<Bingo> bingos;

        while (infile.peek() != EOF)
        {
            std::string line;
            std::getline(infile, line); // blank first
            std::vector<std::string> inputs;
            for (int i = 0; i < 5; i++)
            {
                std::getline(infile, line);
                boost::algorithm::trim(line);
                inputs.push_back(line);
            }
            bingos.push_back(Bingo(inputs));
        }

        std::vector<std::string> draws;
        boost::algorithm::split(draws, draw, boost::is_any_of(","));
        long last = -1;
        for (auto d: draws)
        {
            int p = std::stol(d);
            auto it = bingos.begin();
            while (it != bingos.end())
            {
                if (it->play(p))
                {
                    if (part == 1)
                    {
                        return it->score(p);
                    }
                    else
                    {
                        last = it->score(p);
                        it = bingos.erase(it);
                    }
                } else
                {
                    ++it;
                }
                // b.print();
            }
        }

        return last;
    }

};
