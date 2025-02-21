#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;


template <class T>
class Trie {    
private:
    // Define TrieNode for the first phase (Trie)
    /* DO NOT CHANGE */
    struct TrieNode {
        static const int ALPHABET_SIZE = 128;
        TrieNode* children[ALPHABET_SIZE];
        char keyChar;
        bool isEndOfKey;
        T* data;

        TrieNode(char c) : keyChar(c), isEndOfKey(false), data(NULL) {

                for (int i = 0; i< ALPHABET_SIZE; i++) {
                children[i] = NULL;
                
            }
            
            }
    };

    TrieNode* root;

public: // Do not change.

    Trie(); 
    ~Trie();
    
    Trie& insert(const string& username); 
    void remove(std::string username);
    T* search(std::string username); 
    void findStartingWith(std::string prefix, std::vector<T*> &results); 
    void wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results); 
    void print(); 

private: // you may add your own utility member functions here.
    void deleteTrieNode(TrieNode *node){
        for(int i=0;i<128;i++){
            if((node->children)[i]!=NULL){
                deleteTrieNode((node->children)[i]);
            }
        }
        delete node->data;
        delete node;
    }
    void dataInserter(TrieNode *node,vector<T*> &results){
        if(node){
            if(node->isEndOfKey){
                results.push_back(node->data);
            }
            for(int i=0;i<128;i++){
                if((node->children)[i]!=NULL){
                    dataInserter((node->children)[i],results);
                }
            }
        }
    }
    void asterixHelper(TrieNode *node,vector<TrieNode*> &results,char a){
        if(node){
            if(node->keyChar==a){
                results.push_back(node);
            }
            for(int y=0;y<128;y++){
                asterixHelper((node->children)[y],results,a);
            }
        }
    }//For this one, use a empty vector to store
    void asterixWithQHelper(vector<TrieNode*> &results,char a,int q){
        questionHelper(results,q);
        vector<TrieNode*> newResults;
        for(int l=0;l<results.size();l++){
            asterixHelper(results[l],newResults,a);
        }
        results=newResults;
    }
    void questionHelper(vector<TrieNode*> &results,int q){
        if(q>0){
            vector<TrieNode*> temp;
            for(int m=0;m<results.size();m++){
                for(int n=0;n<128;n++){
                    if((results[m]->children)[n]){
                        temp.push_back((results[m]->children)[n]);
                    }
                }
            }
            results=temp;
            questionHelper(results,q-1);
        }
    }
    void wildHelper(int a, int q, char c, vector<TrieNode*> &results){
        if(a==0&&q==0){
            for(int k=0;k<results.size();k++){
                if(results[k]){
                    results[k]=((results[k])->children)[(int) c];
                }
            }
        }
        else if(a==1&&q==0){
            vector<TrieNode*> newResults;
            for(int k=0;k<results.size();k++){
                asterixHelper(results[k],newResults,c);
            }
            results=newResults;
        }
        else if(a==1&&q!=0){
            asterixWithQHelper(results,c,q);
        }
        else{
            questionHelper(results,q);
            for(int k=0;k<results.size();k++){
                if(results[k]->keyChar!=c){
                    results.erase(results.begin()+k);
                }
            }
        }
    }
    void print(const std::string& primaryKey); // Do not change this line.
    void printTrie(TrieNode* node, const std::string& currentKey); // Do not change this line.
    
};
/* DO NOT CHANGE */
template <class T>
Trie<T>::Trie() : root(new TrieNode('\0')) {}

template <class T>
Trie<T>::~Trie() {
    deleteTrieNode(root);
}

template <class T>
Trie<T>& Trie<T>::insert(const string& key) {
    TrieNode *temp=root;
    int i=0;
    if(key.length()!=0){
        while(key[i]){
            if((temp->children)[(int)key[i]]==NULL){
                (temp->children)[(int)key[i]]= new TrieNode(key[i]);
                temp=(temp->children)[(int)key[i]];
                if(!key[i+1]){
                    temp->isEndOfKey=true;
                }
                temp->data=new T(key);
                i++;
            }
            else{
                temp=(temp->children)[(int)key[i]];
                if(!key[i+1]){
                    temp->isEndOfKey=true;
                }
                i++;
            }
        }
    }
    return *this;
}

template <class T>
T* Trie<T>::search(std::string username) {
    TrieNode *temp=root;
    int i=0;
    if(username.length()!=0){
        while(username[i]){
            temp=(temp->children)[(int)username[i]];
            if(temp==NULL){
                return NULL;
            }
            i++;
        }
        return temp->data;
    }
    else{
        return NULL;
    }
}

template <class T>
void Trie<T>::remove(std::string username) {
    TrieNode *temp=root;
        int i=0;
        while(username[i]){
            temp=(temp->children)[(int)username[i]];
            if(temp==NULL){
                return;
            }
            i++;
        }
        if(temp){
            temp->isEndOfKey=false;
            
        }
}

template <class T>
void Trie<T>::findStartingWith(string prefix,vector<T*> &results) {
    int i=0;
    TrieNode *temp=root;
    while(prefix[i]){
        temp=(temp->children)[(int)prefix[i]];
        i++;
    }
    dataInserter(temp,results);
}

template <class T>
void Trie<T>::wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results) {
    int i=0;
    int a=0;
    int q=0;
    vector<TrieNode*> temp;
    temp.push_back(root);
    while(wildcardKey[i]){
        if(wildcardKey[i]=='*'){
            if(wildcardKey[i+1]){
                a=1;
            }
            else{
                if(q!=0){
                    questionHelper(temp,q);
                }
                for(int j=0;j<temp.size();j++){
                    dataInserter(temp[j],results);
                }
            }
        }
        else if(wildcardKey[i]=='?'){
            if(wildcardKey[i+1]){
                q++;
            }
            else{
                questionHelper(temp,q);
                if(a==1){
                    for(int j=0;j<temp.size();j++){
                        dataInserter(temp[j],results);
                    }
                }
            }
        }
        else{
            wildHelper(a,q,wildcardKey[i],temp);
            q=0;
            a=0;
        }
        i++;
    }
    for(int j=0;j<temp.size();j++){
        if(temp[j]){
            if(temp[j]->isEndOfKey){
                if(!((find(results.begin(), results.end(), temp[j]->data) != results.end()))){
                    results.push_back(temp[j]->data);
                }
            }
        }
    }
    if(results.size()>=2){
        if(results[0]==results[1]){
            results.erase(results.begin()+1);
        }
    }
}

/* DO NOT CHANGE */
template<class T>
void Trie<T>::print() {
    print("");
}
/* DO NOT CHANGE */
template <class T>
void Trie<T>::print(const std::string& primaryKey) {
    if (primaryKey.empty()) {
        // Print the entire tree.
        printTrie(root, "");
    } else {
        // Print specific keys.
        TrieNode* primaryNode = root;
        for (int i = 0; i < primaryKey.length(); i++) {
            
            int index = primaryKey[i];
            if (!primaryNode->children[index]) {
                std::cout << "{}" << std::endl; // Primary key not found, nothing to print.
                return ;
            }
            primaryNode = primaryNode->children[index];
        }

        }
    }
/* DO NOT CHANGE */
template <class T>
void Trie<T>::printTrie(TrieNode* node, const std::string& currentKey) {
    if (!node) {
        return;
    }
    if (node->isEndOfKey) {
        std::cout << currentKey <<std::endl;
        std::cout << "Watched Movies:" << std::endl;
        if(node->data) {
            node->data->printMovies();
        }
    }
    for (int i = 0; i < 128; ++i) {
        if (node->children[i]) {
            printTrie(node->children[i], currentKey + static_cast<char>(i));
        }
    }
}

