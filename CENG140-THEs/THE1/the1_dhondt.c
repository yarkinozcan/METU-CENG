#include <stdio.h>
#include <math.h>
int main() {
    int y=0;
    int c;
    int a;
    int n;
    int m;
    int i;
    int j;
    float max;
    int index;
    float party[26];
    int seats[50];
    int array[50];
    char letters[26]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    scanf("%d %d",&n,&m);
    for(i=0;i<n;i++){
        scanf("%f",&party[i]);
    }
    for(i=0;i<n;i++){
        seats[i]=0;
    }
    for(j=0;j<m;j++){
        max=0;
        index=0;
        for(i=0;i<n;i++){
            if(max<(party[i]/(seats[i]+1))){
                max=party[i]/(seats[i]+1);
                index=i;
            }
            else if((max==(party[i]/(seats[i]+1)))&&(party[i]>party[index])){
                max=party[i]/(seats[i]+1);
                index=i;
            }
        }
        seats[index]+=1;
    }
    for(i=0;i<50;i++){
        array[i]=-1;
    }
    for(i=0;i<n;i++){
        if(seats[i]!=0){
            y+=1;
        }
    }
    for(i=0;i<y;i++){
        max=0;
        index=0;
       for(j=0;j<n;j++){
           c=0;
           if(seats[j]>max){
               for(a=0;a<y;a++){
                   if(array[a]==j){
                       c+=1;
                   }
               }
               if(c==0){
                   max=seats[j];
                   index=j;
               }
           }
       }
       array[i]=index;
       printf("%c: %d\n",letters[index],seats[index]);
    }
    return 0;
}