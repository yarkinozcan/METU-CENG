#include "the6.h"

const int MAX = 200000000;

int find_min(std::vector<int> vec) {
    int size = (int)vec.size();
    int min = vec[0]; 
    for(int i = 1 ; i < size ; i++) {
        min = min > vec[i] ? vec[i] : min;
    }
    return min;
}

//Shortest path bulmak için Bellman-Ford algoritmasını kullanıyoruz
void bellman_ford(std::vector<int>& d, const std::vector< std::vector< std::pair<int, int> > >& network) {
    int size = (int)network.size();
    
    for(int i = 0 ; i < size - 1 ; i++) {
        bool updated = false;
        for(int from = 0 ; from < size ; from++) {
            for(int k = 0 ; k < (int)network[from].size() ; k++) {
                int to = network[from][k].first;
                int weight = network[from][k].second;
                if(d[to] > d[from] + weight){
                    d[to] = d[from] + weight;
                    updated = true;
                }
            }
        }
        if(!updated) break;
    }
}

float friendship_score_calculator(int n1, int n2, std::vector<std::vector<int> >& allPairMatrix) {
    int n1Ton2 = allPairMatrix[n1][n2];
    int n2Ton1 = allPairMatrix[n2][n1];
    float n1Min = find_min(allPairMatrix[n1]);
    float n2Min = find_min(allPairMatrix[n2]);
    
    if(n1Ton2 == MAX && n2Ton1 == MAX) 
        return -1;
    
    if((n1Ton2 == MAX && n2Ton1 != MAX) || (n1Ton2 != MAX && n2Ton1 == MAX))
        return 0;
    
    float score = (n1Min / n1Ton2) * (n2Min / n2Ton1);
    
    return score;
}


std::vector< std::vector<float> > get_friendship_scores(const std::vector< std::vector< std::pair<int, int> > >& network){
    int size = (int)network.size();
    
    // All-pairs shortest path için matrix oluşturma
    std::vector<std::vector<int> > allPairMatrix(size, std::vector<int>(size, MAX)); 
    
    //Matrixi initialize etme
    for(int i = 0 ; i < size ; i++) {
        allPairMatrix[i][i] = 0;
    }
    
    //Shortest Pathlari matrize yazma
    for(int i = 0 ; i < size ; i++) {
        bellman_ford(allPairMatrix[i], network);
    }
    
    for(int i = 0 ; i < size ; i++) {
        allPairMatrix[i][i] = MAX;
    } 
    
    //Shortest Pathlari matrize yazma
    for(int i = 0 ; i < size ; i++) {
        bellman_ford(allPairMatrix[i], network);
    }
    
    //friendshipScoresMatrix' i initialize etme
    std::vector<std::vector<float> > friendshipScoresMatrix(size, std::vector<float>(size, 1));
    
    for(int i = 0 ; i < size ; i++) {
        for(int j = 0 ; j < size ; j++) {
            friendshipScoresMatrix[i][j] = friendship_score_calculator(i, j, allPairMatrix);
        }
    }
    
    return friendshipScoresMatrix;
}
