#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    int h=0;
    h+=(startInt*PRIMES[0]);
    h+=(endInt*PRIMES[1]);
    if(isCostWeighted){
        h+=PRIMES[2];
    }
    return h;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    elementCount=0;
    for(int i=0;i<MAX_SIZE;i++){
        table[i].lruCounter=0;
        table[i].sentinel=EMPTY_MARK;//koddan emin değilim
    }
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    if((elementCount)>MAX_SIZE/2){
        throw TableCapFullException(elementCount);
    }
    if(intArray.size()<1){
        throw InvalidTableArgException();
    }
    int f=intArray[0];
    int l=intArray[intArray.size()-1];
    for(int i=0;i<MAX_SIZE;i++){//checking if the key already exists
        if(table[i].sentinel==OCCUPIED_MARK&&table[i].startInt==f&&table[i].endInt==l&&table[i].isCostWeighted==isCostWeighted){
            table[i].lruCounter+=1;
            return table[i].lruCounter;
        }
    }
    int code=Hash(f,l,isCostWeighted);
    code=code%MAX_SIZE;
    int dummycode=code;
    int y=1;
    while(table[dummycode].sentinel==OCCUPIED_MARK){
        dummycode=(code+y*y);
        dummycode=dummycode%MAX_SIZE;
        y+=1;
    }
    HashData newHashdata;
    newHashdata.intArray=intArray;
    newHashdata.lruCounter=1;
    newHashdata.isCostWeighted=isCostWeighted;
    newHashdata.startInt=f;
    newHashdata.endInt=l;
    newHashdata.sentinel=OCCUPIED_MARK;
    table[dummycode]=newHashdata;
    elementCount+=1;
    return 0;
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].sentinel==OCCUPIED_MARK&&table[i].startInt==startInt&&table[i].endInt==endInt&&table[i].isCostWeighted==isCostWeighted){
            if(incLRU){
                table[i].lruCounter+=1;
            }
            intArray=table[i].intArray;
            return true;
        }
    }
    return false;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].sentinel==SENTINEL_MARK){
            table[i].sentinel==EMPTY_MARK;
        }
        else if(table[i].sentinel==OCCUPIED_MARK){
            table[i].intArray.clear();
            table[i].sentinel=EMPTY_MARK;
        }
    }
    elementCount=0;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
    int lru=0;
    int index;
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].sentinel==OCCUPIED_MARK&&table[i].lruCounter>lru){
            index=i;
            lru=table[i].lruCounter;
        }
    }
    intArray=table[index].intArray;
    
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
     for(int i=0;i<MAX_SIZE;i++){
        if(table[i].sentinel==OCCUPIED_MARK&&table[i].startInt==startInt&&table[i].endInt==endInt&&table[i].isCostWeighted==isCostWeighted){
            intArray=table[i].intArray;
            table[i].intArray.clear();
            table[i].lruCounter=0;
            table[i].sentinel=SENTINEL_MARK;
            elementCount--;
        }
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    int lru;
    int index;
    std::vector<int> array;
    for(int i=0;i<lruElementCount;i++){
        array.clear();
        lru=99;
        for(int j=0;j<MAX_SIZE;j++){
            if(table[j].sentinel==OCCUPIED_MARK&&lru>table[j].lruCounter){
                lru=table[j].lruCounter;
                index=j;
            }
        }
        Remove(array,table[index].startInt,table[index].endInt,table[index].isCostWeighted);
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    std::priority_queue<Pair<int,int>,
                                        std::vector<Pair<int,int>>,
                                        LessComparator<Pair<int,int>>> q;
    Pair<int,int> dummy;
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].sentinel==OCCUPIED_MARK&&table[i].lruCounter!=0){
            dummy.key=table[i].lruCounter;
            dummy.value=i;
            q.push(dummy);
        }
    }
    while(!q.empty()){
        PrintLine(q.top().value);
        q.pop();
    }
}

#endif // HASH_TABLE_HPP