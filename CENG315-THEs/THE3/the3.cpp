#include "the3.h"

// DO NOT CHANGE ABOVE THIS LINE!
// you may implement helper functions below

int find_min_in_vector(const std::vector<int>& vec) {
    int min_val = -1;
    for (int val : vec) {
        if (val < min_val || min_val == -1) {
            min_val = val;
        }
    }
    return min_val;
}

int find_min_cost(const std::vector<std::vector<int>>& costs, int N){
    std::vector<std::vector<int>> total_cost(N-1, std::vector<int>(6, 11));

    for (int j = 0; j < 6; ++j) {
        total_cost[0][j] = costs[0][j];
    }

    for (int i = 1; i < N-1; ++i) {
        total_cost[i][0] = costs[i][0] + find_min_in_vector({total_cost[i - 1][0], total_cost[i - 1][2], total_cost[i - 1][3], total_cost[i - 1][4], total_cost[i - 1][5]});
        total_cost[i][1] = costs[i][1] + find_min_in_vector({total_cost[i - 1][1], total_cost[i - 1][2], total_cost[i - 1][3], total_cost[i - 1][4], total_cost[i - 1][5]});
        total_cost[i][2] = costs[i][2] + find_min_in_vector({total_cost[i - 1][0], total_cost[i - 1][1], total_cost[i - 1][4], total_cost[i - 1][5]});
        total_cost[i][3] = costs[i][3] + find_min_in_vector({total_cost[i - 1][0], total_cost[i - 1][1], total_cost[i - 1][4], total_cost[i - 1][5]});
        total_cost[i][4] = costs[i][4] + find_min_in_vector({total_cost[i - 1][2], total_cost[i - 1][3], total_cost[i - 1][0], total_cost[i - 1][1]});
        total_cost[i][5] = costs[i][5] + find_min_in_vector({total_cost[i - 1][2], total_cost[i - 1][3], total_cost[i - 1][0], total_cost[i - 1][1]});
    }

    return find_min_in_vector(total_cost[N-2]);
}
