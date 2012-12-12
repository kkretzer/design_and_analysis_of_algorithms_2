#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

typedef long Node;

// representing an undirected graph with
// two directed edges for each undirected edge
struct DirectedEdge {
    Node head;
    long cost;
};

typedef std::vector<DirectedEdge> EdgeList;

DirectedEdge cheapest_boundary_edge(std::map<Node,EdgeList> &X, std::map<Node,EdgeList> &V);
bool edge_comparator(DirectedEdge &a, DirectedEdge &b);

int main(int argc, char** argv)
{
  //std::ifstream input("test.txt"); // total cost should be 230
  std::ifstream input("edges.txt");  // total cost should be -3612829
  unsigned long num_nodes, num_edges;
  long long total_cost = 0;
  Node n1, n2;
  long c;

  std::map<Node,EdgeList> X, V;

  input >> num_nodes >> num_edges;
  while (input >> n1 >> n2 >> c) {
    if(V.find(n1) == V.end()) {
        EdgeList edges;
        V.insert(std::make_pair<Node,EdgeList>(static_cast<Node>(n1), static_cast<EdgeList>(edges)));
    }
    if(V.find(n2) == V.end()) {
        EdgeList edges;
        V.insert(std::make_pair<Node,EdgeList>(static_cast<Node>(n2), static_cast<EdgeList>(edges)));
    }
    DirectedEdge e1{n2,c}, e2{n1,c};
    V[n1].push_back(e1);
    V[n2].push_back(e2);
  }

  X.insert(*V.begin());
  V.erase(V.begin());

  while (!V.empty()) {
    const DirectedEdge &min_edge = cheapest_boundary_edge(X, V);
    total_cost += min_edge.cost;
    auto head_node_iter = V.find(min_edge.head);
    X.insert(*head_node_iter);
    V.erase(head_node_iter);
  }

  std::cout << "cost of the MST is " << total_cost << std::endl;
  return 0;
}

DirectedEdge cheapest_boundary_edge(std::map<Node,EdgeList> &X, std::map<Node,EdgeList> &V) {
  EdgeList x_edges;
  for (auto &node : X) {
    for (auto &edge : node.second) {
      if (V.find(edge.head) != V.end()) {
        x_edges.push_back(edge);
      }
    }
  }
  return *std::min_element(x_edges.begin(), x_edges.end(), edge_comparator);
}

bool edge_comparator(DirectedEdge &a, DirectedEdge &b) {
  return a.cost < b.cost;
}

