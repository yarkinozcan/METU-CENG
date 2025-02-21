#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "the3.h"

Cache* createCache()
{
    Cache *new_cache=(Cache *)malloc(sizeof(Cache));
    int i;
    int j;
    int k;
    int size=0;
    DomainFreqNode *prev;
    DomainFreqNode *curr;
    char *token;
    char string[1000];
    CacheNode *array[1000];
    DomainFreqList *domlist_curr;
    DomainFreqNode *domnode_curr;
    scanf("%d %d\n", &(i),&(k));
    new_cache->cacheLimit=i;
    new_cache->mediaCount=k;
    new_cache->head=NULL;
    i=0;
    k=0;
    for(i=0;i<new_cache->mediaCount;i++){
        array[i]=(CacheNode *)malloc(sizeof(CacheNode));
        domlist_curr=(DomainFreqList *)malloc(sizeof(DomainFreqList));
        domnode_curr=(DomainFreqNode *)malloc(sizeof(DomainFreqNode));
        token=NULL;
        fgets(string,1000,stdin);
        j=0;
        k=0;
        token=strtok(string," ");
        domlist_curr->head=NULL;
        while(token!=NULL){
            if(j==0){
                array[i]->media.name = (char*)malloc((strlen(token) + 1)*sizeof(char));
                strcpy(array[i]->media.name, token);
                j+=1;
            }
            else if(j==1){
                array[i]->media.size=atoi(token);
                j+=1;
                size+=array[i]->media.size;
            }
            else{
                if(k==2){
                    domnode_curr=(DomainFreqNode *)malloc(sizeof(DomainFreqNode));
                    k=0;
                }
                if(k==0){
                    domnode_curr->name = (char*)malloc((strlen(token) + 1)*sizeof(char));
                    strcpy(domnode_curr->name, token);
                    k+=1;
                }
                else if(k==1){
                    k+=1;
                    domnode_curr->freq=atoi(token);
                    curr=domlist_curr->head;
                    prev=NULL;
                    while(curr&&(((curr->freq)>(domnode_curr->freq))||((curr->freq)==(domnode_curr->freq)&&strcmp(curr->name,domnode_curr->name)<0))){
                        prev=curr;
                        curr=curr->next;
                    }
                    if(prev==NULL&&curr==NULL){
                        domnode_curr->next=NULL;
                        domnode_curr->prev=NULL;
                        domlist_curr->head=domnode_curr;
                        domlist_curr->tail=domnode_curr;
                    }
                    else if(prev==NULL){
                        domnode_curr->next=curr;
                        curr->prev=domnode_curr;
                        domnode_curr->prev=NULL;
                        domlist_curr->head=domnode_curr;
                    }
                    else if(curr==NULL){
                        domnode_curr->next=NULL;
                        domnode_curr->prev=prev;
                        prev->next=domnode_curr;
                        domlist_curr->tail=domnode_curr;
                    }
                    else{
                        domnode_curr->next=curr;
                        curr->prev=domnode_curr;
                        prev->next=domnode_curr;
                        domnode_curr->prev=prev;
                    }
                } 
            }
            token=strtok(NULL, " ");
        }
        array[i]->domainFreqList=domlist_curr;
    }
    for(i=0;i<new_cache->mediaCount;i++){
        if(i==0){
            array[i]->prev=NULL;
            array[i]->next=NULL;
            new_cache->head=array[i];
            new_cache->tail=array[i];
        }
        else if(i==new_cache->mediaCount-1){
            array[i]->next=NULL;
            array[i]->prev=array[i-1];
            array[i-1]->next=array[i];
            new_cache->tail=array[i];
        }
        else{
            array[i]->prev=array[i-1];
            array[i-1]->next=array[i];
        }
    }
    new_cache->currentSize=size;
    return new_cache;
}

/*
    Prints cache information and its contents
*/
void printCache(Cache* cache)
{
    int i;
    CacheNode *curr;
    DomainFreqNode *dom_curr;
    if(cache->currentSize==0){
        printf("The Cache is Empty");
    }
    else{
        printf("-------------- Cache Information --------------\n");
        printf("Cache Limit: %d KB\n",cache->cacheLimit);
        printf("Current Size: %d KB\n",cache->currentSize);
        printf("Media Count: %d\n",cache->mediaCount);
        printf("Cache Media Info:\n");
        curr=cache->head;
        for(i=0;i<cache->mediaCount;i++){
            printf("    Name: %s    Size: %d KB\n",curr->media.name,curr->media.size);
            dom_curr=(curr->domainFreqList)->head;
            printf("    ------- Media Domain Frequencies -------\n");
            while(dom_curr){
                printf("        Name: %s    Freq: %d\n",dom_curr->name,dom_curr->freq);
                dom_curr=dom_curr->next;
            }
            printf("    ----------------------------------------\n");
            curr=curr->next;
        }
        printf("----------------------------------------------");
    }
}


/*
    Adds media to the cache
*/
CacheNode* addMediaRequest(Cache* cache, Media media, char* domain)
{
    DomainFreqNode *domnode_curr;
    DomainFreqNode *domnode_prev=NULL;
    CacheNode *newcache;
    DomainFreqList *newdomlist;
    DomainFreqNode *newdomnode;
    CacheNode *curr;
    CacheNode *prev=NULL;
    int media_exist=0;
    int domain_exist=0;
    curr=cache->head;
    while(curr){
        if(strcmp(curr->media.name,media.name)==0){
            if(curr->prev!=NULL){
                if(curr->next==NULL){
                    curr->prev->next=NULL;
                    curr->prev=NULL;
                    curr->next=cache->head;
                    cache->head->prev=curr;
                    cache->head=curr;
                }
                else{
                    curr->prev->next=curr->next;
                    curr->next->prev=curr->prev;
                    curr->prev=NULL;
                    curr->next=cache->head;
                    cache->head->prev=curr;
                    cache->head=curr;
                }
            }
            media_exist=1;
            domnode_curr=curr->domainFreqList->head;
            while(domnode_curr){
                if(strcmp(domnode_curr->name,domain)==0){
                    domnode_curr->freq+=1;
                    domain_exist=1;
                    domnode_prev=domnode_curr->prev;
                    while(domnode_prev&&((domnode_curr->freq>domnode_prev->freq)||(domnode_curr->freq==domnode_prev->freq&&strcmp(domnode_curr->name,domnode_prev->name)<0))){
                        if(domnode_curr->next==NULL){
                            curr->domainFreqList->tail=domnode_prev;
                        }
                        domnode_prev->next=domnode_curr->next;
                        domnode_curr->prev=domnode_prev->prev;
                        domnode_curr->next=domnode_prev;
                        domnode_prev->prev=domnode_curr;
                        domnode_prev=domnode_curr->prev;
                    }
                    if(domnode_curr->prev==NULL){
                        curr->domainFreqList->head=domnode_curr;
                    }
                    return curr;
                }
                else{
                    domnode_prev=domnode_curr;
                    domnode_curr=domnode_curr->next;
                    
                }
            }
            domnode_prev=NULL;
            if(!domain_exist){
                newdomnode=(DomainFreqNode *)malloc(sizeof(DomainFreqNode));
                newdomnode->name=(char *)malloc(sizeof(char)*strlen(domain));
                strcpy(newdomnode->name,domain);
                newdomnode->freq=1;
                domnode_curr=curr->domainFreqList->head;
                while(domnode_curr&&(((domnode_curr->freq)>(newdomnode->freq))||((domnode_curr->freq)==(newdomnode->freq)&&strcmp(domnode_curr->name,domnode_curr->name)<0))){
                    domnode_prev=domnode_curr;
                    domnode_curr=domnode_curr->next;
                }
                if(domnode_prev==NULL&&domnode_curr==NULL){
                    newdomnode->next=NULL;
                    newdomnode->prev=NULL;
                    curr->domainFreqList->head=newdomnode;
                    curr->domainFreqList->tail=newdomnode;
                }
                else if(domnode_prev==NULL){
                    newdomnode->next=domnode_curr;
                    domnode_curr->prev=newdomnode;
                    newdomnode->prev=NULL;
                    curr->domainFreqList->head=newdomnode;
                }
                else if(domnode_curr==NULL){
                    newdomnode->next=NULL;
                    newdomnode->prev=domnode_prev;
                    domnode_prev->next=newdomnode;
                    curr->domainFreqList->tail=newdomnode;
                }
                else{
                    newdomnode->next=domnode_curr;
                    domnode_curr->prev=newdomnode;
                    domnode_prev->next=newdomnode;
                    newdomnode->prev=domnode_prev;
                }
            }
            return curr;
        }
        else{
            curr=curr->next;
        }
    }
    if(!media_exist){
        while(media.size+cache->currentSize>cache->cacheLimit){
            cache->mediaCount-=1;
            cache->currentSize-=cache->tail->media.size;
            if(cache->tail->prev!=NULL){
            cache->tail=cache->tail->prev;
            cache->tail->next=NULL;}
            else{
                cache->tail=NULL;
                cache->head=NULL;
            }
        }
        cache->currentSize+=media.size;
        cache->mediaCount+=1;
        newcache=(CacheNode *)malloc(sizeof(CacheNode));
        newdomlist=(DomainFreqList *)malloc(sizeof(DomainFreqList));
        newdomnode=(DomainFreqNode *)malloc(sizeof(DomainFreqNode));
        newdomnode->name=(char *)malloc(sizeof(char)*strlen(domain));
        strcpy(newdomnode->name,domain);
        newdomnode->freq=1;
        newdomnode->prev=NULL;
        newdomnode->next=NULL;
        newdomlist->head=newdomnode;
        newdomlist->tail=newdomnode;
        newcache->media=media;
        newcache->domainFreqList=newdomlist;
        newcache->prev=NULL;
        if(cache->head!=NULL){
            newcache->next=cache->head;
            cache->head->prev=newcache;
            cache->head=newcache;}
        else{
            newcache->next=NULL;
            cache->head=newcache;
            cache->tail=newcache;
        }
        return newcache;
    }
}

/*
    Finds the corresponding media's node in the cache
    Return NULL if it is not found
*/
CacheNode* findMedia(Cache* cache, char* name)
{
    CacheNode *curr;
    curr=cache->head;
    while(curr){
        if(strcmp(curr->media.name,name)==0){
            return curr;
        }
        else{
            curr=curr->next;
        }
    }
    return NULL;
}

/*
    Finds a media from its name and deletes it's node from the cache
    Do not do anything if media is not found
*/
void deleteMedia(Cache* cache, char* name)
{
    CacheNode *curr;
    curr=cache->head;
    while(curr){
        if(strcmp(curr->media.name,name)==0){
            if(curr->prev==NULL&&curr->next==NULL){
                cache->head=NULL;
                cache->tail=NULL;
            }
            else if(curr->prev==NULL){
                curr->next->prev=NULL;
                cache->head=curr->next;
            }
            else if(curr->next==NULL){
                curr->prev->next=NULL;
                cache->tail=curr->prev;
            }
            else{
                curr->prev->next=curr->next;
                curr->next->prev=curr->prev;
            }
            cache->mediaCount-=1;
            cache->currentSize-=curr->media.size;
            break;
        }
        else{
            curr=curr->next;
        }
    }
}

