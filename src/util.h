#pragma once

#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

struct readers
{
    // this is to read a file that is newline delimited into a container of type T
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

    template <typename T>
    struct Caster
    {
        int operator()(const std::string& str) { return boost::lexical_cast<T>(str); }
    };

    // this is to read a file that is a single line, delimited by commas or spaces, into a container of type T
    template<typename T>
    static void read_delimited_line(const std::string& filename, T& result)
    {
        std::ifstream infile(filename);
        std::string line;
        std::getline(infile, line);

        boost::tokenizer<> tokens(line);
        std::transform(tokens.begin(), tokens.end(), std::back_inserter(result), Caster<typename T::value_type>());
    }

};
