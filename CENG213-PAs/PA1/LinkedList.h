#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &rhs);
    ~LinkedList();

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

    int getSize() const;
    bool isEmpty() const;
    bool containsNode(Node<T> *node) const;
    int getIndex(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void append(const T &data);
    void prepend(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAtIndex(const T &data, int index);
    void moveToIndex(int currentIndex, int newIndex);
    void mergeNodes(int sourceIndex, int destIndex);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeNodeAtIndex(int index);
    void removeAllNodes();

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    int size;
};

template<class T>
LinkedList<T>::LinkedList()
{
    T newData;
    head= new Node<T>(newData);
    size=0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &rhs)
{   
    T newData;
    head= new Node<T>(newData);
    size=rhs.getSize();
    int i;
    Node<T> *current=head;
    for(i=0;i<rhs.getSize();i++){
        current->next=new Node<T>(rhs.getNodeAtIndex(i)->data,current,NULL);
        current=current->next;
    }
}

template<class T>
LinkedList<T>::~LinkedList()
{   
    Node<T> *first=head;
    Node<T> *second;
    while(first){
        second=first->next;
        delete first;
        first=second;
    }
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    if(this!=&rhs){
        removeAllNodes();
        size=rhs.getSize();
        int i;
        Node<T> *current=head;
        for(i=0;i<rhs.getSize();i++){
        current->next=new Node<T>(rhs.getNodeAtIndex(i)->data,current,NULL);
        current=current->next;
        }
    }
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    /*Node<T> *first=head->next;
    int i=0;
    while(first){
        first=first->next;
        i++;
    }
    return i;*/
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const
{
    if(head->next){
        return false;
    }
    else{
        return true;
    }
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    Node<T> *first=head;
    while(first){
        if(first==node){
            return true;
        }
        first=first->next;
    }
    return false;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    if(isEmpty()){
        return NULL;
    }
    else{
        return head->next;
    }
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    if(isEmpty()){
        return NULL;
    }
    else{
        Node<T> *last=head;
        while(last->next){
            last=last->next;
        }
        return last;
    }
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
    Node<T> *first=head->next;
    if(isEmpty()){
        return NULL;
    }
    else{
        while(first){
            if(first->data==data){
                return first;
            }
            first=first->next;
        }
        return NULL;
    }
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    Node<T> *first=head->next;
    int i;
    if(index>=size||index<0){
        return NULL;
    }
    else{
        for(i=0;i<index;i++){
            first=first->next;
        }
        return first;
    }
}

template<class T>
void LinkedList<T>::append(const T &data)
{   
    if(isEmpty()){
        Node<T> *last= new Node<T>(data,head,NULL);
        head->next=last;
        size++;
    }
    else{
        Node<T> *plast= getLastNode();
        Node<T> *last= new Node<T>(data,plast,NULL);
        plast->next=last;
        size++;
    }
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    Node<T> *pfirst=getFirstNode();
    Node<T> *first= new Node<T>(data,head,getFirstNode());
    head->next=first;
    if(size!=0){
        pfirst->prev=first;
    }
    size++;
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    Node<T> *current= head;
    while(current){
        if(current==node){
            Node<T> *newNode=new Node<T>(data);
            newNode->next=current->next;
            if(newNode->next){
                newNode->next->prev=newNode;
            }
            current->next=newNode;
            newNode->prev=current;
        }
        current=current->next;
    }
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    if(index<size&&index>=0){
        Node<T> *current= head;
        int i;
        for(i=0;i<index;i++){
            current=current->next;
        }
        Node<T> *newNode= new Node<T>(data,current,current->next);
        if(current->next){
            current->next->prev=newNode;
        }
        current->next=newNode;
    }
    size++;
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    int i=0;
    int j=-1;
    Node<T> *current= head->next;
    while(current){
        if(current==node){
            j=i;
        }
        current=current->next;
        i++;
    }
    return j;
}

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    if(currentIndex<size&&currentIndex>=0){
        Node<T> *current= head->next;
        Node<T> *newNode= head->next;
        Node<T> *newNode2= head;
        int i;
        if(newIndex>currentIndex){
            for(i=0;i<currentIndex;i++){
                current=current->next;
            }
            if(newIndex<size){
                for(i=0;i<newIndex;i++){
                newNode=newNode->next;
                }
            }
            else{
                while(newNode->next){
                    newNode=newNode->next;
                }
            }
            current->prev->next=current->next;
            if(current->next){
                current->next->prev=current->prev;
            }
            current->prev=newNode;
            current->next=newNode->next;
            if(newNode->next){
                newNode->next->prev=current;
            }
            newNode->next=current;
        }
        else if(newIndex<currentIndex){
            for(i=0;i<currentIndex;i++){
                current=current->next;
            }
            if(newIndex<size){
                for(i=0;i<newIndex;i++){
                newNode2=newNode2->next;
                }
            }
            else{
                while(newNode2->next){
                    newNode2=newNode2->next;
                }
            }
            current->prev->next=current->next;
            if(current->next){
                current->next->prev=current->prev;
            }
            current->prev=newNode2;
            current->next=newNode2->next;
            if(newNode2->next){
                newNode2->next->prev=current;
            }
            newNode2->next=current;
        }
    }
}


template<class T>
void LinkedList<T>::mergeNodes(int sourceIndex, int destIndex)
{
    Node<T> *current= head->next;
    Node<T> *dest= head->next;
    int i;
    if(sourceIndex>=0&&sourceIndex<size&&destIndex>=0&&destIndex<size){
        for(i=0;i<sourceIndex;i++){
            current=current->next;
        }
        for(i=0;i<destIndex;i++){
            dest=dest->next;
        }
        Node<T> *prev= current->prev;
        prev->next=current->next;
        if(current->next){
        current->next->prev=prev;
        }
        dest->data=dest->data+current->data;
        delete current;
        size--;
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{   
    Node<T> *current= head;
    while(current){
        if(current==node){
            current->prev->next=current->next;
            if(current->next){
                current->next->prev=current->prev;
            }
            delete current;
            size--;
            break;
        }
        current=current->next;
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{   
    Node<T> *current= head->next;
    Node<T> *depo;
    while(current){
        if(current->data==data){
            depo=current->next;
            removeNode(current);
            current=depo;
        }
        else{
            current=current->next;
        }
    }
}

template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    int i;
    if(index<size&&index>=0){
        Node<T> *current= head->next;
        for(i=0;i<index;i++){
            current=current->next;
        }
        removeNode(current);
    }
}

template<class T>
void LinkedList<T>::removeAllNodes()
{   
    Node<T> *current= head->next;
    Node<T> *nextt;
    while(current){
        nextt=current->next;
        removeNode(current);
        current=nextt;
    }
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    while(node)
    {
        std::cout << *node << std::endl;
        node = node->next;
    }

}

#endif //LINKEDLIST_H