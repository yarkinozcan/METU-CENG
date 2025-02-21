#include <iostream>
#include <fstream>
#include <chrono>
#include "the7.h"

void print_network(std::vector<std::vector<std::pair<int,int> > >& network) {
    int node_number = (int) network.size();
    if (node_number == 0) {
        std::cout << "There is no node in the network" << std::endl;
        return;
    }

    for (int idx=0; idx < node_number; idx++) {
        std::cout << idx << ":\t{";
        for (const auto& edge : network[idx]) {
            std::cout << " (" << edge.first << ", " << edge.second << ") ";
        }
        std::cout << "} " << std::endl;
    }
}


void read_from_file(std::vector<std::vector<std::pair<int, int> > >& network,
                    int& starting_node, int& destination_node){
    int node_number, edge_number;
    char addr[]= "inp01.txt"; // 01-10 are available
    std::ifstream infile (addr);
    if (!infile.is_open()){
        std::cout << "File \'"<< addr
                  << "\' can not be opened. Make sure that this file exists." << std::endl;
        return;
    }
    infile >> node_number >> edge_number;
    infile >> starting_node >> destination_node;
    network.resize(node_number);
    for(int idy=0; idy < edge_number; idy++) {
        int source, dest, weight;
        infile >> source >> dest >> weight;
        network[source].push_back(std::make_pair(dest, weight));
    }
    infile.close();
}

void print_path(const std::vector<std::vector<std::pair<int, int> > >& graph,
                        std::vector<int> path, long path_length){
    std::cout << "longest path length: " << path_length << std::endl << path[0];

    for(int idx=1; idx < (int)path.size(); idx++){
        int edge_weight=0;
        for(const auto& edge : graph[path[idx-1]])
            if(edge.first == path[idx]) edge_weight = edge.second;
        std::cout << " - (" << edge_weight << ") -> "<< path[idx];
    }
    std::cout << std::endl;
}


int main() {
    std::vector<std::vector<std::pair<int, int> > > graph;
    int starting_node;
    int destination_node;
    read_from_file(graph, starting_node, destination_node);
    std::cout << "road graph:" << std::endl;
    print_network(graph);
    std::cout << "starting node: "<< starting_node << std::endl << "destination node: " << destination_node << std::endl;
    long length_of_longest_path = 0;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> longest_path = find_longest_path(graph, starting_node, destination_node, length_of_longest_path);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    std::cout << std::endl << "execution time: " << duration.count() << " us" << std::endl;
    print_path(graph, longest_path, length_of_longest_path);

    return 0;
}