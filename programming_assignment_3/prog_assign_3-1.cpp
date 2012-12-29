#include <iostream>
#include <fstream>
#include <vector>

struct item {
    unsigned int value;
    unsigned int weight;
};

int main(int argc, char** argv)
{
    std::ifstream input("knapsack1.txt");
    unsigned int knapsack_size, num_items;
    unsigned int value, weight;
    std::vector<item> items;
    items.push_back({0,0}); // array 'A' has a dummy first entry, keep this in sync

    input >> knapsack_size >> num_items;

    unsigned int A[num_items+1][knapsack_size+1];
    for (unsigned int x = 0; x <= knapsack_size; ++x) {
        A[0][x] = 0;
    }

    while (input >> value >> weight) {
        items.push_back({value, weight});
    }

    for (unsigned int i = 1; i <= num_items; ++i) {
        for (unsigned int x = 0; x <= knapsack_size; ++x) {
            unsigned int without_this_item = A[i-1][x];
            unsigned int with_this_item = x >= items[i].weight ? A[i-1][x-items[i].weight] + items[i].value : 0;
            A[i][x] = without_this_item > with_this_item ? without_this_item : with_this_item;
        }
    }

    std::cout << "optimal solution is " << A[num_items][knapsack_size] << std::endl;

    return 0;
}
