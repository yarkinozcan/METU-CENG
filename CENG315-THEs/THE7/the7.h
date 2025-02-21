#ifndef THE7_THE7_H
#define THE7_THE7_H

#include<vector>
#include<stack>
#include<utility>
#include<climits>
#include<algorithm>
#include<memory>

std::vector<int> find_longest_path(const std::vector<std::vector<std::pair<int, int> > >& graph,
                                                       int starting_node,
                                                       int destination_node,
                                                       long& length_of_longest_path);

#endif //THE7_THE7_H


