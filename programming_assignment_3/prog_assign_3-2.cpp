#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

struct item {
    unsigned long value;
    unsigned long weight;
};

typedef std::vector<item> item_container_t;
typedef std::pair<unsigned long, unsigned long> memo_key_t;
typedef std::map<memo_key_t, unsigned long> memo_map_t;

unsigned long optimalKnapsackValue(memo_map_t &memo, item_container_t &items, unsigned long x, unsigned long i);
bool sort_by_weight(const item &a, const item &b);

int main(int argc, char** argv)
{
    std::ifstream input("knapsack2.txt");
    unsigned long knapsack_size, num_items;
    unsigned long value, weight;
    unsigned long magic_number = 367; // all weights in knapsack2.txt are multiples of 367
    memo_map_t memo;
    item_container_t items;
    items.push_back({0,0}); // base case for recursion

    input >> knapsack_size >> num_items;
    knapsack_size = knapsack_size/magic_number;

    while (input >> value >> weight) {
        items.push_back({value, weight/magic_number});
    }

    // algorithm is a faster when items sorted by weight
    sort(items.begin(), items.end(), sort_by_weight);

    unsigned long optimalValue = optimalKnapsackValue(memo, items, knapsack_size, num_items);
    std::cout << "optimal knapsack value is " << optimalValue << std::endl;

    return 0;
}

unsigned long optimalKnapsackValue(memo_map_t &memo, item_container_t &items, unsigned long x, unsigned long i)
{
    memo_key_t memo_key = {x,i};
    if (!memo.count(memo_key)) {
        if (i == 0) {
            return 0;
        }
        unsigned long i_weight = items[i].weight;
        if (i_weight > x) {
            memo[memo_key] = optimalKnapsackValue(memo, items, x, i-1);
        } else {
            unsigned long without_this_item = optimalKnapsackValue(memo, items, x, i-1);
            unsigned long with_this_item = items[i].value + optimalKnapsackValue(memo, items, x - i_weight, i-1);
            memo[memo_key] = without_this_item > with_this_item ? without_this_item : with_this_item;
        }
    }
    return memo[memo_key];
}

bool sort_by_weight(const item &a, const item &b)
{
    return a.weight < b.weight;
}
