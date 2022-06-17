#pragma once

#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

struct readers
{
    // this is to read a file that is newline delimited POD types into a container of type T
    // uses boost::lexical_cast to convert to the inner type
    template <typename T>
    static void read_by_line(const std::string& filename, T& result)
    {
        std::ifstream infile(filename);
        std::string line;
        while (std::getline(infile, line))
        {
            result.push_back(boost::lexical_cast<typename T::value_type>(line));
        }
    }

    // this is to read a file that is newline delimited, but each line isn't a simple POD type
    // pass in a functor that does the parsing and returns the object inside the container
    template <typename T, typename F>
    static void read_by_line(const std::string& filename, F functor, T& result)
    {
        std::ifstream infile(filename);
        std::string line;
        while (std::getline(infile, line))
        {
            result.push_back(functor(line));
        }
    }

    template <typename T>
    struct Caster
    {
        int operator()(const std::string& str) { return boost::lexical_cast<T>(str); }
    };

    // this is to read a file that is a single line of PODs, delimited by commas or spaces, into a container of type T
    template <typename T>
    static void read_delimited_line(const std::string& filename, T& result)
    {
        std::ifstream infile(filename);
        std::string line;
        std::getline(infile, line);

        boost::tokenizer<> tokens(line);
        std::transform(tokens.begin(), tokens.end(), std::back_inserter(result), Caster<typename T::value_type>());
    }

    // this is for 2-dimensional character matrices
    template <typename T, typename F>
    static void read_dense_2d_matrix(const std::string& filename, F functor, T& result)
    {
        std::ifstream infile(filename);
        std::string line;
        size_t x = 0, y = 0;
        while (std::getline(infile, line))
        {
            for (x = 0; x < line.length(); x++)
            {
                result[x][y] = functor(line[x]);
            }
            y++;
        }
    }

};
