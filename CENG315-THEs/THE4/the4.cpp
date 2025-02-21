#include "the4.h"

// DO NOT CHANGE ABOVE THIS LINE!
// You may implement helper functions below

unsigned int alice_cutting_cake(const unsigned short cake_width,
                                const unsigned short cake_height,
                                bool **perfect_cuts){
    
    int dp[601][601];
    
    for(int i = 1;i <= cake_width;i++){
        for(int j = 1; j <= cake_height;j++) {
            if(perfect_cuts[i][j]) {
                dp[i][j] = 0;
            }
            else {
                dp[i][j] = i*j;
            }
        }
    }
    
    for(int i = 1; i <= cake_width; i++) {
        for(int j = 1; j <= cake_height;j++) {
            
            if(dp[i][j] == 0){
                continue;
            }
            
            for(int l = 1;l <= i;l++){
                
                if(dp[i][j] == 0){
                    break;
                }
                
                dp[i][j] = std::min(dp[i][j],dp[l][j] + dp[i-l][j]);
                
            }
            
            for(int l = 1;l <= j;l++){
                
                if(dp[i][j] == 0){
                    break;
                }
                
                dp[i][j] = std::min(dp[i][j], dp[i][l] + dp[i][j-l]);
                
            }
        }
    }
    return dp[cake_width][cake_height];
}
