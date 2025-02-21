#include <stdio.h>
#include <stdlib.h>
int path(char **p,int x,int y,int rows,int columns, int a[]){
    int i;
    int k;
    if(p[y][x]!=' '){
        return 0;
    }
    p[y][x]='.';
    if((x==columns-1)||(y==rows-1)||(x==0)||(y==0)){
        for(i=0;i<rows;i++){
            for(k=0;k<columns;k++){
                if(p[i][k]=='.'){
                    p[i][k]='*';
                }
            }
        }
        for(i=0;i<rows;i++){
            if(i!=0){
                printf("\n");
            }
            for(k=0;k<columns;k++){
                printf("%c",p[i][k]);
                if((k==columns-1)&&(i==rows-1)){
                    printf("\n");
                }
            }
        }
        a[0]=1;
        return 1;
    }
    path(p,x+1,y,rows,columns,a);
    if(a[0]==1){
        return 1;
    }
    path(p,x-1,y,rows,columns,a);
    if(a[0]==1){
        return 1;
    }
    path(p,x,y+1,rows,columns,a);
    if(a[0]==1){
        return 1;
    }
    path(p,x,y-1,rows,columns,a);
    if(a[0]==1){
        return 1;
    }
}
int main() {
    char a[110000];
    char **array;
    char j;
    int i=0;
    int k=0;
    int rows=0;
    int columns=0;
    int x;
    int l=0;
    int y;
    int e[1];
    e[0]=0;
    scanf("%d %d",&x,&y);
    while(j!=EOF){
        j=getchar();
        if(j=='\n'){
            rows+=1;
        }
        a[i]=j;
        i+=1;
    }
    a[i-1]='\0';
    rows-=1;
    columns=i/rows-1;
    array=(char **)malloc(sizeof(char *)*rows);
    for(i=0;i<rows;i++){
        array[i]=(char *)malloc(sizeof(char)*columns);
    }
    for(i=0;i<rows;i++){
        for(k=0;k<columns;k++){
            if(a[l]=='\n'){
                array[i][k]=a[l+1];
                l+=2;
            }
            else{
                array[i][k]=a[l];
                l+=1;
            }
        }
    }
    path(array,x,y,rows,columns,e);
    if(e[0]==0){
        for(i=0;i<rows;i++){
            if(i!=0){
                printf("\n");
            }
            for(k=0;k<columns;k++){
                printf("%c",array[i][k]);
                if((k==columns-1)&&(i==rows-1)){
                    printf("\n");
                }
            }
        }
    }
    return 0;
}