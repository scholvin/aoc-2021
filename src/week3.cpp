#include "week3.h"
#include "util.h"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

namespace week3
{
    struct coordinate_t
    {
        int x, y;
        bool operator==(const coordinate_t& other) const { return x == other.x && y == other.y; }
    };

    struct coordinate_hash {
        std::size_t operator() (const coordinate_t &coordinate) const
        {
            return std::hash<int>()(coordinate.x) ^ std::hash<int>()(coordinate.y);
        }
    };

    struct vertex_prop_t { coordinate_t coordinates; };
    struct edge_prop_t { int weight; };
    using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vertex_prop_t, edge_prop_t>;
    using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
    typedef std::unordered_map<coordinate_t, vertex_t, coordinate_hash> vertex_map_t;

    template <int SIZE>
    void populate_graph(const std::array<std::array<uint8_t, SIZE>, SIZE>& grid, graph_t& graph, vertex_map_t& vmap)
    {
         /* The grid represents the edge weight *into* that node.
            For example:

                456
                789

            In this graph, the edge from [0][1] to [0][0] has weight 4, and the edge from [0][0] to [0][1] is 5.

            So now we go through the usual rigamarole of populating a Boost graph based on this. Vertex
            identifiers will be a tuple of the {x,y} location (coordinate_t).
        */

        // the usual stupid mapping problem - start with creating/inserting all the vertices
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                vmap[{x,y}] = boost::add_vertex(vertex_prop_t{x,y}, graph);
            }
        }

        // for neighbors
        const std::vector<coordinate_t> offsets = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
        // now we add edges
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                for (int dx = -1; dx <= 1; dx += 2)
                {
                    for (auto offset: offsets)
                    {
                        int xn = x + offset.x;
                        int yn = y + offset.y ;
                        if (xn >= 0 && yn >= 0 && xn < SIZE && yn < SIZE)
                        {
                            // xn, yn is a neighbor of x,y and it's in the graph
                            // there are two edges, one in each direction, and the weight is the grid value at the destination
                            boost::add_edge(vmap[{x,y}], vmap[{xn,yn}], edge_prop_t{grid[xn][yn]}, graph);
                            boost::add_edge(vmap[{xn,yn}], vmap[{x,y}], edge_prop_t{grid[x][y]}, graph);
                        }
                    }
                }
            }
        }
    }

    long day15a()
    {
#if 0
        const int SIZE = 10;
        const std::string FILENAME = "../data/day15-smol.dat";
        const int SANE = 6;
#else
        const int SIZE = 100;
        const std::string FILENAME = "../data/day15.dat";
        const int SANE = 7;
#endif

        std::array<std::array<uint8_t, SIZE>, SIZE> grid;
        readers::read_dense_2d_matrix(FILENAME, readers::digit_parser(), grid);

        graph_t graph;
        vertex_map_t vmap;
        populate_graph<SIZE>(grid, graph, vmap);

        // sanity check
        auto x = boost::edge(vmap[{1,0}], vmap[{2,0}], graph);
        assert(x.second && graph[x.first].weight == SANE);

        std::vector<int> distances(boost::num_vertices(graph));
        vertex_t start = vmap[{0, 0}];
        boost::dijkstra_shortest_paths(graph, start,
                                       boost::weight_map(boost::get(&edge_prop_t::weight, graph))
                                            .distance_map(boost::make_iterator_property_map(distances.begin(),
                                                          boost::get(boost::vertex_index, graph))));
        vertex_t finish = vmap[{SIZE-1,SIZE-1}];
        return distances[finish];
    }

    long day15b()
    {
        const int MULT = 5;
#if 0
        const int GSIZE = 10;
        const std::string FILENAME = "../data/day15-smol.dat";
        const int SANE = 6;
#else
        const int GSIZE = 100;
        const std::string FILENAME = "../data/day15.dat";
        const int SANE = 7;
#endif
         const int SIZE = GSIZE * MULT;

        std::array<std::array<uint8_t, GSIZE>, GSIZE> little_grid;
        readers::read_dense_2d_matrix(FILENAME, readers::digit_parser(), little_grid);

        std::array<std::array<uint8_t, SIZE>, SIZE> grid { 0 };

        // i,j is the big grid (5x5) of the smaller (GSIZE x GSIZE) grids
        // copy them, increasing values as we go right and down (weird mod 9)
        for (int i = 0; i < MULT; i++)
        {
            for (int j = 0; j < MULT; j++)
            {
                for (int x = 0; x < GSIZE; x++)
                {
                    for (int y = 0; y < GSIZE; y++)
                    {
                        grid[x + i*GSIZE][y + j*GSIZE] = little_grid[x][y] + i + j;
                        if (grid[x + i*GSIZE][y + j*GSIZE] > 9)
                        {
                            grid[x + i*GSIZE][y + j*GSIZE] -= 9;
                        }
                    }
                }
            }
        }
#if 0
        for (int y = 0; y < SIZE; y++)
        {
            for (int x = 0; x < SIZE; x++)
            {
                std::cout << static_cast<int>(grid[x][y]);
            }
            std::cout << std::endl;
        }
#endif
        graph_t graph;
        vertex_map_t vmap;
        populate_graph<SIZE>(grid, graph, vmap);

        std::vector<int> distances(boost::num_vertices(graph));
        vertex_t start = vmap[{0, 0}];
        boost::dijkstra_shortest_paths(graph, start,
                                       boost::weight_map(boost::get(&edge_prop_t::weight, graph))
                                            .distance_map(boost::make_iterator_property_map(distances.begin(),
                                                          boost::get(boost::vertex_index, graph))));
        vertex_t finish = vmap[{SIZE-1,SIZE-1}];
        return distances[finish];
    }
};
