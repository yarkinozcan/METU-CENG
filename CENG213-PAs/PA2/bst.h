#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
using namespace std;

// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class BST {
private:
    // Define TreeNode for the second phase (Binary Search Tree)
    /* DO NOT CHANGE */
    struct TreeNode {
        std::string key;
        T data;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const std::string &k, const T& value) : key(k), data(value), left(NULL), right(NULL) {}
        
    };

    TreeNode* root;


public: // Do not change.

    BST();
    ~BST();
    TreeNode* getRoot() { return root; }
    bool isEmpty() { return root == NULL; }

    BST& insert(const std::string key, const T& value);
    bool search(std::string value) const;
    void remove(std::string value);
    BST<T>* merge(BST<T>* bst);
    BST<T>* intersection(BST<T>* bst);
    std::vector<TreeNode> tree2vector(TreeNode* root);
    void print();
    
private:// you may add your own utility member functions here.
    

    void print(TreeNode* node, std::string indent, bool last, bool isLeftChild); // Do not change.
    void tree2vector(TreeNode* node, vector<TreeNode>& result){
        if(node!=NULL){
            tree2vector(node->left,result);
            result.push_back(TreeNode(node->key, node->data));
            tree2vector(node->right,result);
        }
    }
    void deleteBST(TreeNode *node){
        if(node){
            deleteBST(node->left);
            deleteBST(node->right);
            if(node){
                delete node;
            }
        }
    }
    void insertBST(TreeNode *node, TreeNode *first){
        if(first!=NULL){
            if(node->key.compare(first->key)<0){
                if(first->left){
                    insertBST(node,first->left);
                }
                else{
                    first->left=node;
                }
            }
            else if(node->key.compare(first->key)>0){
                if(first->right){
                    insertBST(node,first->right);
                }
                else{
                    first->right=node;
                }
            }
        }
    }
    void mergeInsert(TreeNode *&root,TreeNode node){
        if(root==NULL){
            root=&node;
        }
        else{
            if(root->key<node.key){
                if(root->right==NULL){
                    root->right=&node;
                }
                else{
                    mergeInsert(root->right,node);
                }
            }
            else if(root->key>node.key){
                if(root->left==NULL){
                    root->left=&node;
                }
                else{
                    mergeInsert(root->left,node);
                }
            }
        }
    }
    bool searchBST(std::string value, TreeNode *node) const{
        if(node==NULL){
            return false;
        }
        if(value.compare(node->key)==0){
            return true;        
        }
        if(value.compare(node->key)<0){
            return searchBST(value,node->left);
        }
        if(value.compare(node->key)>0){
            return searchBST(value,node->right);
        }
    }
    void removeNode(TreeNode *&r,std::string key){
       if(r!=NULL){
            if(key.compare(r->key)<0){
                removeNode(r->left,key);
            }
            else if(key.compare(r->key)>0){
                removeNode(r->right,key);
            }
            else{
                if(r->right!=NULL&&r->left!=NULL){
                    r->key=getMin(r->right)->key;
                    r->data=getMin(r->right)->data;
                    removeNode(r->right,r->key);
                }
                else{
                    if(r->left!=NULL){
                        r->key=(r->left)->key;
                        r->data=(r->left)->data;
                        removeNode(r->left,r->key);
                    }
                    else if(r->right!=NULL){
                        r->key=(r->right)->key;
                        r->data=(r->right)->data;
                        removeNode(r->right,r->key);
                    }
                    else{
                        TreeNode *oldNode=r;
                        r=NULL;
                        delete oldNode;
                    }
                }
            }
        }
    }
    TreeNode *getMin(TreeNode *node){
        if(node->left==NULL){
            return node;
        }
        else{
            return getMin(node->left);
        }
    }
    bool containsTree(TreeNode *r,std::string value){
        if(r==NULL){
            return false;
        }
        if(r->key.compare(value)<0){
            return containsTree(r->left,value);
        }
        else if(r->key.compare(value)>0){
            return containsTree(r->right,value);
        }
        else{
            return true;
        }
    }
    
};

    // Constructor
    template <class T>
    BST<T>::BST() : root(NULL) {}

    // Destructor
    template <class T>
    BST<T>::~BST() {
    deleteBST(root);
    }

    template <class T>
    BST<T>& BST<T>::insert(const string key, const T& value) {
        if(root){
            if(!containsTree(root,key)){
                TreeNode *node = new TreeNode(key, value);
                insertBST(node,root);
                return *this;
            }
        }
        else{
            root = new TreeNode(key, value);
            return *this;
        }
    }
    
    template <class T>
    bool BST<T>::search(std::string value) const {
        return searchBST(value,root);
    }
    
    // Remove a node from BST for given key. If key not found, do not change anything.
    template <class T>
    void BST<T>::remove(std::string key) {
        if(!search(key))
            return;
        removeNode(root,key);
    }
    
    
    // A helper function for converting a BST into vector.
    template <class T>
    vector<typename BST<T>::TreeNode> BST<T>::tree2vector(TreeNode* node){
        vector<TreeNode> result;
        tree2vector(node,result);
        return result;
    }
    
    // Merge two BST's and return merged BST.
    template <class T>
    BST<T>* BST<T>::merge(BST<T>* bst){
        if(bst->getRoot()==NULL&&root==NULL){
            return NULL;
        }
        else{
            vector<TreeNode> result=tree2vector(root);
            vector<TreeNode> result2=tree2vector(bst->getRoot());
            int j=0;
            int k=0;
            vector<TreeNode> result3;
            while(j+k<result.size()+result2.size()){
                if(j>=result.size()){
                    result3.push_back(result2[k]);
                    k++;
                }
                else if(k>=result2.size()){
                    result3.push_back(result[j]);
                    j++;
                }
                else if(result[j].key<result2[k].key){
                    result3.push_back(result[j]);
                    j++;
                }
                else{
                    result3.push_back(result2[k]);
                    k++;
                }
            }
            BST<T>* newBST= new BST<T>;
            for(int i=0;i<result3.size();i++){
                newBST->insert(result3[i].key,result3[i].data);
            }
            return newBST;
        }
    }
        
    // Intersect two BST's and return new BST.
    template <class T>
    BST<T>* BST<T>::intersection(BST<T>* bst) {
        if(bst->getRoot()==NULL&&root==NULL){
            return NULL;
        }
        else{
            vector<TreeNode> result=tree2vector(root);
            vector<TreeNode> result2=tree2vector(bst->getRoot());
            int j=0;
            int k=0;
            vector<TreeNode> result3;
            while(true){
                if(j>=result.size()||k>=result2.size()){
                    break;
                }
                else if(result[j].key<result2[k].key){
                    j++;
                    continue;
                }
                else if(result[j].key>result2[k].key){
                    k++;
                }
                else{
                    result3.push_back(result[j]);
                    j++;
                    k++;
                }
            }
            if(result3.size()==0){
                return NULL;
            }
            BST<T>* newBST= new BST<T>;
            for(int i=0;i<result3.size();i++){
                newBST->insert(result3[i].key,result3[i].data);
            }
            return newBST;
        }
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print() {
        print(root, "", true, false);
    
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print(TreeNode* node, string indent, bool last, bool isLeftChild) {
        if (node != NULL) {
            cout << node->key << endl;
            print(node->left, indent, false, true);
            print(node->right, indent, true, false);
        }
    
    }


