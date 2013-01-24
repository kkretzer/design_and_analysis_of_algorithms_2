#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <limits>

typedef std::pair<float,float> coordinate;
typedef std::pair<coordinate,coordinate> path;
struct hasher {
    size_t operator()(const path &p) const
    {
        return (std::hash<float>()(p.first.first) ^ std::hash<float>()(p.first.second))
            + (std::hash<float>()(p.second.first) ^ std::hash<float>()(p.second.second));
    }
};

std::unordered_map<unsigned long, std::vector<unsigned long>> essSets(unsigned long limit);
typedef std::bitset<sizeof(unsigned long)*8> essSet;

int main(int argc, char **argv)
{
    std::ifstream input(argv[1]);
    unsigned num_cities;
    float x, y;
    std::vector<coordinate> cities;
    std::unordered_map<path,float,hasher> path_lengths;
    float infinity = std::numeric_limits<float>::infinity();

    input >> num_cities;

    unsigned long s_set_count = std::pow(2,num_cities-1);
    float **A = new float*[s_set_count];
    for (unsigned long s = 0; s < s_set_count; ++s) {
        A[s] = new float[num_cities];
        for (unsigned long i = 0; i < num_cities; ++i) {
            if (i == 0 && s == 0) {
                A[s][i] = 0;
            } else {
                A[s][i] = infinity;
            }
        }
    }

    while (input >> x >> y) {
        coordinate c({x,y});
        for (auto &city : cities) {
            float x_diff = c.first - city.first;
            float y_diff = c.second - city.second;
            float length = std::sqrt((x_diff*x_diff) + (y_diff*y_diff));
            path_lengths[{c,city}] = length;
            path_lengths[{city,c}] = length;
        }
        cities.push_back(c);
    }

    std::unordered_map<unsigned long, std::vector<unsigned long>> sets = essSets(s_set_count);

    for (unsigned long m = 2; m <= num_cities; ++m) {
        for (auto &s : sets[m]) {
            essSet S(s);
            for (unsigned long j = 1; j < num_cities; ++j) {
                if (S[j] == 1) {
                    float min_val = infinity;
                    for (unsigned long k = 0; k < num_cities; ++k) {
                        if (S[k] == 1 && k != j) {
                            essSet S_without_j = S;
                            S_without_j[j] = 0;
                            float pl = path_lengths[{cities[k],cities[j]}];
                            float l = A[S_without_j.to_ulong() >> 1][k] + pl;
                            if (l < min_val) {
                                min_val = l;
                            }
                        }
                    }
                    // 1st dimension of the 'A' array is indexed by 'S' without the first bit
                    A[S.to_ulong() >> 1][j] = min_val;
                }
            }
        }
    }

    float min_val = infinity;
    for (unsigned long j = 1; j < num_cities; ++j) {
        float pl = path_lengths[{cities[j],cities[0]}];
        float l = A[s_set_count-1][j] + pl;
        if (l < min_val) {
            min_val = l;
        }
    }

    std::cout << "The minimum cost of a traveling salesman tour is " << min_val << std::endl;

    return 0;
}

std::unordered_map<unsigned long, std::vector<unsigned long>> essSets(unsigned long limit)
{
    std::unordered_map<unsigned long, std::vector<unsigned long>> sets;

    for (unsigned long i = 0; i < limit; ++i) {
        unsigned long j = (i << 1) | 1; // this algo requires city '1' to be in each set
        essSet S(j);
        size_t ones_count = S.count();
        if (!sets.count(ones_count)) {
            std::vector<unsigned long> v;
            sets[ones_count] = v;
        }
        sets[ones_count].push_back(j);
    }

    return sets;
}
