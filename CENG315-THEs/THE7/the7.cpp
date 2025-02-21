#include "the7.h"

void dfs(const std::vector<std::vector<std::pair<int, int>>>& graph, 
         int node, 
         int destination_node, 
         long current_length, 
         std::vector<bool>& visited, 
         std::vector<int>& current_path, 
         std::vector<int>& best_path, 
         long& max_length) {
    if (node == destination_node) {
        if (current_length > max_length) {
            max_length = current_length;
            best_path = current_path;
        }
        return;
    }

    visited[node] = true;

    for (int i = 0; i < graph[node].size(); i++) {
        int neighbor = graph[node][i].first;
        int weight = graph[node][i].second;
        if (!visited[neighbor]) {
            current_path.push_back(neighbor);
            dfs(graph, neighbor, destination_node, current_length + weight, visited, current_path, best_path, max_length);
            current_path.pop_back();
        }
    }

    visited[node] = false;
}

std::vector<int> find_longest_path(const std::vector<std::vector<std::pair<int, int> > >& graph,
                                   int starting_node,
                                   int destination_node,
                                   long& length_of_longest_path) {
    int n = graph.size();
    std::vector<int> topo_order;
    std::vector<int> in_degree(n, 0);

    for (int u = 0; u < n; u++) {
        for (int i = 0; i < graph[u].size(); i++) {
            int v = graph[u][i].first;
            in_degree[v]++;
        }
    }

    std::stack<int> zero_in_degree;
    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0) {
            zero_in_degree.push(i);
        }
    }

    while (!zero_in_degree.empty()) {
        int node = zero_in_degree.top();
        zero_in_degree.pop();
        topo_order.push_back(node);

        for (int i = 0; i < graph[node].size(); i++) {
            int v = graph[node][i].first;
            in_degree[v]--;
            if (in_degree[v] == 0) {
                zero_in_degree.push(v);
            }
        }
    }

    if (topo_order.size() != n) {
        length_of_longest_path = -1000000000;
        std::vector<bool> visited(n, false);
        std::vector<int> current_path;
        std::vector<int> best_path;

        current_path.push_back(starting_node);
        dfs(graph, starting_node, destination_node, 0, visited, current_path, best_path, length_of_longest_path);

        if (length_of_longest_path == -1000000000) {
            return {};
        }

        return best_path;
    }

    std::vector<long> dist(n, -1000000000);
    std::vector<int> parent(n, -1);
    dist[starting_node] = 0;

    bool start_found = false;
    for (int i = 0; i < topo_order.size(); i++) {
        int u = topo_order[i];
        if (u == starting_node) {
            start_found = true;
        }
        if (!start_found || dist[u] == -1000000000) {
            continue;
        }

        for (int j = 0; j < graph[u].size(); j++) {
            int v = graph[u][j].first;
            int weight = graph[u][j].second;
            if (dist[u] + weight > dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    if (dist[destination_node] == -1000000000) {
        length_of_longest_path = -1;
        return {};
    }

    length_of_longest_path = dist[destination_node];
    std::vector<int> path;
    int current = destination_node;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    std::reverse(path.begin(), path.end());
    return path;
}