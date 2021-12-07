# AOC 2021

This is my code for the [Advent of Code 2021 challenge](https://adventofcode.com/2021).

I'm not really making any effort for this code to be efficient. Just trying to get the answers right.

I'm testing this with gcc 8.5 and cmake 3.20.

* It'll probably work with a lot of other compilers. 
* I set the experimental C++20 flag, but it's unlikely I'll use any of those features.

## To build:
```
mkdir build
cd build
cmake ../src
make
```

This generates the `aoc-2021` executable in the build directory. Run it from there, as it will look to `../data` for input files.
Command line help is available from the executable.

## For an optimized build:
```
mkdir bfast
cd bfast
cmake ../src -DCMAKE_BUILD_TYPE=Release
make
```

Command line help is available from the executable.

