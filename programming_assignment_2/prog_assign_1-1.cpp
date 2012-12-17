#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <memory>

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
    std::ifstream input("clustering1.txt");
    //std::ifstream input("test.txt");
    unsigned int node1, node2, distance, num_nodes;
    unsigned int target_num_clusters = 4, num_clusters;

    std::map<unsigned int, node_ptr> leaders;
    std::vector<edge> edges;

    input >> num_nodes;
    num_clusters = num_nodes;
    while (input >> node1 >> node2 >> distance) {
        edge e = {node1, node2, distance};
        edges.push_back(e);
        node n1 = {node1, {}};
        node n2 = {node2, {}};
        leaders[node1] = std::make_shared<node>(n1);
        leaders[node2] = std::make_shared<node>(n2);
    }
    
    std::sort(edges.begin(), edges.end(), edge_comp);

    while (num_clusters > target_num_clusters) {
        edge shortest = edges.back();
        edges.pop_back();
        node_ptr l1 = leaders[leaders[shortest.node_num_1]->leader];
        node_ptr l2 = leaders[leaders[shortest.node_num_2]->leader];
        while (l1->leader == l2->leader) {
            shortest = edges.back();
            edges.pop_back();
            l1 = leaders[leaders[shortest.node_num_1]->leader];
            l2 = leaders[leaders[shortest.node_num_2]->leader];
        }
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

    edge shortest = edges.back();
    edges.pop_back();
    node_ptr l1 = leaders[leaders[shortest.node_num_1]->leader];
    node_ptr l2 = leaders[leaders[shortest.node_num_2]->leader];
    while (l1->leader == l2->leader) {
        shortest = edges.back();
        edges.pop_back();
        l1 = leaders[leaders[shortest.node_num_1]->leader];
        l2 = leaders[leaders[shortest.node_num_2]->leader];
    }
    std::cout << "The maximum spacing with " << num_clusters << " clusters is " << shortest.distance << std::endl;

    return 0;
}
