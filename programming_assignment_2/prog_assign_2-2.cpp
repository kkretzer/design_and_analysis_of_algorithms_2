#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <memory>
#include <map>

struct InputNode {
    unsigned int label;
    std::vector<unsigned short> bits;
    unsigned short ones_count;
};

typedef std::vector<InputNode> InputList;

bool input_node_comp(const InputNode &in1, const InputNode &in2)
{
    return in1.ones_count < in2.ones_count;
}

struct node {
    unsigned int leader;
    std::vector<unsigned int> followers;
};

typedef std::shared_ptr<node> node_ptr;

struct edge {
    unsigned int node_num_1;
    unsigned int node_num_2;
    unsigned int distance;
};

bool edge_comp(const edge &e1, const edge &e2) {
    return e1.distance > e2.distance;
}


int main(int argc, char** argv)
{
    //std::ifstream input("five.txt");          // should be 1
    //std::ifstream input("tiny.txt");          // should be 4
    //std::ifstream input("seven.txt");         // should be 3
    //std::ifstream input("medium.txt");        // should be 45
    std::ifstream input("clustering2.txt");   // should be ???

    unsigned int num_nodes, node_length, input_counter;
    unsigned short bit;

    InputList input_nodes;
    unsigned int num_clusters;

    std::map<unsigned int, node_ptr> leaders;
    std::vector<edge> edges;

    input >> num_nodes >> node_length;
    num_clusters = num_nodes;
    input_counter = 0;
    std::vector<unsigned short> bits = {};
    unsigned short ones_count = 0;
    while (input >> bit && ++input_counter <= node_length) {
        bits.push_back(bit);
        if (bit == 1) {
            ++ones_count;
        }
        if (input_counter == node_length) {
            input_counter = 0;
            InputNode input_node = {static_cast<unsigned int>(input_nodes.size()), bits, ones_count};
            input_nodes.push_back(input_node);
            bits = {};
            ones_count = 0;
        }
    }

    std::sort(input_nodes.begin(), input_nodes.end(), input_node_comp);

    for (auto iter = input_nodes.begin(); iter != input_nodes.end(); ++iter) {
        auto comp_iter = iter;
        ++comp_iter;
        for (; comp_iter != input_nodes.end() && ((*comp_iter).ones_count - (*iter).ones_count) <= 2; ++comp_iter) {
            unsigned int dist = 0;
            for (unsigned int i = 0; i < node_length; ++i) {
                if ((*iter).bits[i] != (*comp_iter).bits[i]) {
                    ++dist;
                }
            }
            if (dist < 3) {
                unsigned int node1 = (*iter).label, node2 = (*comp_iter).label;
                edge e = {node1, node2, dist};
                edges.push_back(e);
                node n1 = {node1, {}};
                node n2 = {node2, {}};
                leaders[node1] = std::make_shared<node>(n1);
                leaders[node2] = std::make_shared<node>(n2);
            }
        }
    }

    std::sort(edges.begin(), edges.end(), edge_comp);

    /*
    for (auto &e : edges) {
        std::cout << e.node_num_1 << " " << e.node_num_2 << " " << e.distance << std::endl;
    }
    */


    while (!edges.empty() && num_clusters > 1) {

     /*
     std::cout << std::endl << std::endl << std::endl;

     for (auto &n : leaders) {
         std::cout << n.first << " :: " << n.second->leader << std::endl;
     }
     */

        edge shortest = edges.back();
        edges.pop_back();
        node_ptr l1 = leaders[leaders[shortest.node_num_1]->leader];
        node_ptr l2 = leaders[leaders[shortest.node_num_2]->leader];
        while (l1->leader == l2->leader && !edges.empty()) {
            shortest = edges.back();
            edges.pop_back();
            l1 = leaders[leaders[shortest.node_num_1]->leader];
            l2 = leaders[leaders[shortest.node_num_2]->leader];
        }
        if (l1->leader != l2->leader) {
            if (l1->followers.size() <= l2->followers.size()) {
                while (!l1->followers.empty()) {
                    unsigned int n = l1->followers.back();
                    leaders[n]->leader = l2->leader;
                    l2->followers.push_back(n);
                    l1->followers.pop_back();
                }
                l2->followers.push_back(l1->leader);
                l1->leader = l2->leader;
            } else {
                while (!l2->followers.empty()) {
                    unsigned int n = l2->followers.back();
                    leaders[n]->leader = l1->leader;
                    l1->followers.push_back(n);
                    l2->followers.pop_back();
                }
                l1->followers.push_back(l2->leader);
                l2->leader = l1->leader;
            }
            --num_clusters;
        }
    }

    std::cout << "With maximum spacing of 3, the number of clusters is " << num_clusters << std::endl;

    return 0;
}
