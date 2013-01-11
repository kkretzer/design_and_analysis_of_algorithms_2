#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <ctime>

#define infinity LONG_MAX

unsigned long createMatrix(long ***&A, char *input_file);
void destroyMatrix(long ***&A, unsigned long num_vertices);

int main(int argc, char** argv)
{
    long shortest_shortest_path = infinity;  // shortest path of all shortest paths in the graph

    long ***A;
    unsigned long num_vertices = createMatrix(A, argv[1]);

    for (unsigned long k = 1; k <= num_vertices; ++k) {
        for (unsigned long i = 1; i <= num_vertices; ++i) {
            for (unsigned long j = 1; j <= num_vertices; ++j) {
                // 'A' is NxNx2 since we only need to store the 'current k' and 'previous k'
                // mod k by 2 to alternate between the two available k levels of A
                unsigned long current_k  = k%2;
                unsigned long previous_k = (k-1)%2;
                long previous_k_distance = A[i][j][previous_k];

                // use pure k value when indexing into the i or j levels
                long i_to_k = A[i][k][previous_k];
                long k_to_j = A[k][j][previous_k];
                long current_k_distance = (i_to_k == infinity || k_to_j == infinity) ?  infinity : i_to_k + k_to_j;

                A[i][j][current_k] = previous_k_distance < current_k_distance ? previous_k_distance : current_k_distance;

                if (A[i][j][current_k] < shortest_shortest_path) {
                    shortest_shortest_path = A[i][j][current_k];
                }

                if (k == num_vertices && i == j && A[i][j][current_k] < 0) {
                    std::cout << "this graph contains a negative cycle!" << std::endl;
                    return 1;
                }
            }
        }
    }

    destroyMatrix(A, num_vertices);

    std::cout << "The shortest shortest path is " << shortest_shortest_path << std::endl;

    return 0;
}

unsigned long createMatrix(long ***&A, char *input_file)
{
    std::ifstream input(input_file);
    unsigned long num_vertices, num_edges;
    input >> num_vertices >> num_edges;

    // create a NxNx2 array 'A'
    // do not need NxNxN, only need to know previous 'k' values
    A = new long**[num_vertices+1];
    for (unsigned long i = 0; i <= num_vertices; ++i) {
        A[i] = new long*[num_vertices+1];
        for (unsigned long j = 0; j <= num_vertices; ++j) {
            A[i][j] = new long[2];

            if (i == j) {
                // path length from vertex to itself is zero
                A[i][j][0] = 0;
            } else {
                // otherwise assume no edge from i to j
                A[i][j][0] = infinity;
            }
            A[i][j][1] = infinity;
        }
    }

    // fill in edge lengths
    unsigned long tail, head;
    long length;
    while (input >> tail >> head >> length) {
        A[tail][head][0] = length;
    }

    return num_vertices;
}

void destroyMatrix(long ***&A, unsigned long num_vertices)
{
    for (unsigned long i = 1; i <= num_vertices; ++i) {
        for (unsigned long j = 1; j <= num_vertices; ++j) {
            delete[] A[i][j];
        }
        delete[] A[i];
    }
    delete[] A;
}
