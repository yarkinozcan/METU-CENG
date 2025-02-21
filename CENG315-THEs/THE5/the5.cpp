#include "the5.h"

const int WHITE = 0;
const int GRAY = 1;
const int BLACK = 2;

void dfs(int node, const std::vector<std::vector<bool>> &graph,
         std::vector<int> &color, std::vector<int> &finishStack,
         std::vector<int> &currentSCC, bool collectSCC) {

    color[node] = GRAY;

    for (int neighbor = 0; neighbor < graph.size(); ++neighbor) {
        if (graph[node][neighbor] && color[neighbor] == WHITE) {
            dfs(neighbor, graph, color, finishStack, currentSCC, collectSCC);
        }
    }

    color[node] = BLACK;

    if (collectSCC) {
        currentSCC.push_back(node); 
    } else {
        finishStack.push_back(node); 
    }
}

std::vector<std::vector<int>> findSCCs(const std::vector<std::vector<bool>> &dependencies) {
    int n = dependencies.size();
    std::vector<int> color(n, WHITE);
    std::vector<int> finishStack;

    for (int node = 0; node < n; ++node) {
        if (color[node] == WHITE) {
            std::vector<int> dummy;
            dfs(node, dependencies, color, finishStack, dummy, false);
        }
    }

    std::vector<std::vector<bool>> transposedGraph(n, std::vector<bool>(n, false));
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (dependencies[u][v]) {
                transposedGraph[v][u] = true;
            }
        }
    }

    for(int i = 0;i < color.size();i++) {
        color[i] = WHITE;
    }
    
    std::vector<std::vector<int>> sccs;

    for (int i = finishStack.size() - 1; i >= 0; --i) {
        int current = finishStack[i];
        if (color[current] == WHITE) {
            std::vector<int> currentSCC;
            dfs(current, transposedGraph, color, finishStack, currentSCC, true);
            sccs.push_back(currentSCC);
        }
    }

    return sccs;
}

std::vector<std::vector<int>> buildSCCGraph(const std::vector<std::vector<bool>> &dependencies,
                                            const std::vector<std::vector<int>> &sccs) {

    std::vector<int> nodeToSCC(dependencies.size(), -1);

    for (int i = 0; i < sccs.size(); ++i) {
        for (int node : sccs[i]) {
            nodeToSCC[node] = i;
        }
    }

    std::vector<std::vector<int>> sccGraph(sccs.size());

    for (int u = 0; u < dependencies.size(); ++u) {
        for (int v = 0; v < dependencies.size(); ++v) {
            if (dependencies[u][v]) {
                int sccU = nodeToSCC[u], sccV = nodeToSCC[v];
                if (sccU != sccV) {
                    sccGraph[sccU].push_back(sccV);
                }
            }
        }
    }

    return sccGraph;
}

std::vector<int> topologicalSort(const std::vector<std::vector<int>> &sccGraph, const std::vector<int> &sccIds) {
    int n = sccGraph.size();
    std::vector<int> inDegree(n, 0);

    for (const auto &neighbors : sccGraph) {
        for (int v : neighbors) {
            inDegree[v]++;
        }
    }

    std::vector<int> zeroInDegree;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            zeroInDegree.push_back(i);
        }
    }

    std::vector<int> order;

    while (!zeroInDegree.empty()) {
        int minNode = zeroInDegree[0];
        int minIdx = 0;
        for (int i = 1; i < zeroInDegree.size(); ++i) {
            if (sccIds[zeroInDegree[i]] < sccIds[minNode]) {
                minNode = zeroInDegree[i];
                minIdx = i;
            }
        }

        order.push_back(minNode);
        zeroInDegree.erase(zeroInDegree.begin() + minIdx);

        for (int v : sccGraph[minNode]) {
            inDegree[v]--;
            if (inDegree[v] == 0) {
                zeroInDegree.push_back(v);
            }
        }
    }

    return order;
}

std::vector<std::vector<int>> find_work_order(const std::vector<std::vector<bool>> &dependencies) {
    std::vector<std::vector<int>> sccs = findSCCs(dependencies);

    std::vector<int> sccIds(sccs.size());
    for (int i = 0; i < sccs.size(); ++i) {
        int minId = sccs[i][0];
        for (int j = 1; j < sccs[i].size(); ++j) {
            if (sccs[i][j] < minId) {
                minId = sccs[i][j];
            }
        }
        sccIds[i] = minId; // Assigning work packages with the smallest ID
    }

    std::vector<std::vector<int>> sccGraph = buildSCCGraph(dependencies, sccs);

    std::vector<int> topOrder = topologicalSort(sccGraph, sccIds);

    std::vector<std::vector<int>> workOrder;
    for (int id : topOrder) {
        workOrder.push_back(sccs[id]);
    }

    return workOrder;
}
