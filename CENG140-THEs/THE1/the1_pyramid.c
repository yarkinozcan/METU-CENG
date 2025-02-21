#include <stdio.h>
double hesap(int row, int column, double block_weight){
    if(row==0){
        return 0;
    }
    else if(column==0){
        return hesap(row-1,0,block_weight)*0.15+block_weight/2;
    }
    else if(column==row){
        return hesap(row-1,row-1,block_weight)*0.15+block_weight/2;
    }
    else{
        return hesap(row-1,column-1,block_weight)*0.15+hesap(row-1,column,block_weight)*0.15+block_weight;
    }
}


double calculate_pressure_on_block(int row, int column, double block_weight){
    if(row==0){
        printf("(0,0) %.8f\n",hesap(row,column,block_weight));
    }
    else if(column==0){
        calculate_pressure_on_block(row-1,row-1,block_weight);
        printf("(%d,%d) %.8f\n",row,column,hesap(row,column,block_weight));
    }
    else if(column==row){
        calculate_pressure_on_block(row,column-1,block_weight);
        printf("(%d,%d) %.8f\n",row,column,hesap(row,column,block_weight));
    }
    else{
        calculate_pressure_on_block(row,column-1,block_weight);
        printf("(%d,%d) %.8f\n",row,column,hesap(row,column,block_weight));
    }
}


int main() {
    int n;
    int m;
    scanf("%d %d",&n,&m);
    calculate_pressure_on_block(n-1,n-1,m); 
    return 0;
}