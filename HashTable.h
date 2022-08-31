

#ifndef WET2_WINTER2020_HASHTABLE_H
#define WET2_WINTER2020_HASHTABLE_H
#include "linkedlist.h"
template<class Data>
class hashTable{
public:
    linkedList<int,Data> *table;
    int size,thickness;
    int elements;
    hashTable() : table(new linkedList<int ,Data>[2]), size(2),thickness(0),elements(0){}

int hash(int key){
    return key % size;
}

void resize(int new_size){
    linkedList<int, Data> *curr = table;
    thickness = 0;
    int curr_size = size;
    size = new_size;
    table = new linkedList<int , Data>[new_size];
    for (int i = 0; i < curr_size; ++i) {
         while (curr[i].getSize() !=0){
             //updating the thickness of the resized table
             if (table[hash(curr[i].getFirst()->key)].getSize() == 0){
                 thickness++;
             }
            //copying the first node in the list in the i'th place in the table.
            table[hash(curr[i].getFirst()->key)].addBeforeHead(curr[i].getFirst()->key,curr[i].getFirst()->data);
            //removing the that node after we have coyed it
            curr[i].deleteNode(curr[i].getFirst());
         }
    }
    delete [] curr;
}
void expand(){
        resize(2*(this->size));
    }
void reduce(){
        resize(int (this->size/2));
    }

typename :: linkedList<int , Data> :: Node findNode(int key){
    return table[hash(key)].getNodeByKey(key);
    }
bool isContain(int key){
        if(findNode(key) == nullptr){
            return false;
        }
    return true;
    }
bool insert(int key , Data data){
    if (isContain(key)){
        return false;
    }
    //updating the thickness
    if (table[hash(key)].getSize() == 0){
        thickness++;
    }
    // adding the new object to the table
    table[hash(key)].addBeforeHead(key,data);
    // now checking if we need to expand our table acording to the thickness for adding more objects in the future.
    if (thickness == size ){
        expand();
    }
    elements++;
    return true;
    }
bool remove(int key){
        if (!isContain(key)){
            return false;
        }
        typename :: linkedList<int , Data> :: Node to_remove = findNode(key);
        // removing the node from the table
        table[hash(key)].deleteNode(to_remove);
    //updating the thickness of the resized table
    if (table[hash(key)].getSize() == 0){
        thickness--;
    }
    // now checking if we need to expand our table acording to the thickness and the current size.
    if (thickness <= int(size/4)){
        reduce();
    }
    elements--;
    return true;

    }

    void destroyHash(){
        delete [] table;
    }
    
    Data getData(int key){
        return findNode(key)->data;
    }
    void setScore(int key,int score){
        typename :: linkedList<int , Data> :: Node itr = findNode(key);
        itr->data.score = score;
    }
    void increaseLevel(int key,int level){
        typename :: linkedList<int , Data> :: Node itr = findNode(key);
        itr->data.level += level;
    }
    void UnionHash(hashTable& other_hash){
        for (int i=0; i<other_hash.size; i++) {
            typename :: linkedList<int , Data> :: Node itr = other_hash.table[i].getFirst();
            if(itr==nullptr){
                continue;
            }
            do {
                this->insert(itr->key,itr->data);
                itr = other_hash.table[i].getNext();
            } while (itr);
        }
    }
    ~hashTable(){
        delete [] table;
    }
};

#endif //WET2_WINTER2020_HASHTABLE_H
