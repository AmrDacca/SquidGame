
#ifndef WET2_WINTER2020_LINKEDLIST_H
#define WET2_WINTER2020_LINKEDLIST_H
#include <iostream>
using  namespace std ;


template <class Key, class Data>
class linkedList {

public:
    typedef class node{
    public :
        Key key;
        Data data;
        node* next;
        node* prev;

        node(Key key, Data data, node* next=nullptr, node* prev=nullptr) : key(key), data(data), next(next), prev(prev){};
        ~node()=default;
    }*Node;

public:
    int size;
    Node head;
    Node tail;
    Node iterator;

public :
    //Basic fours, constructor, copy cot's, dcot, operator =.

    //(1)
    linkedList():size(0), head(nullptr), tail(nullptr), iterator(nullptr){};

    //(2)
    ~linkedList(){
        // that indicates that the lis is allredy empty
        if(this->head== nullptr)
            return;
        destroyList();
    }
    //(3)
    linkedList &operator= (const linkedList& to_copy){
        if (this == &to_copy) {
            return *this;
        }
        this->destroyList();
        Node ite=to_copy.tail;
        while (ite) {
            this->addBeforeHead(ite->key,ite->data);
            ite=ite->prev;
        }
        return *this;
    }
    //(4)
    linkedList(const linkedList& to_copy):size(0), head(nullptr), tail(nullptr), iterator(nullptr){
        Node ite=to_copy.tail;
        while (ite){
            addBeforeHead(ite->key,ite->data);
            ite=ite->prev;
        }

    }


//adding a new node with the data and the key after the node before and returning the poiter on the new node in our list
    //()
    Node addAfter(Node before,Key key_to_add, Data data_to_add){
        Node new_node;
        //in this case we  there is no node to add after it.
        if(before== nullptr)return nullptr;
        //creating the new node to add
        if (before->next != nullptr) {
            new_node = new node(key_to_add, data_to_add, before->next, before);
        } else{
            new_node = new node(key_to_add, data_to_add, nullptr, before);
        }
        //adding after the tail.
        if (before==tail){
            tail=new_node;
        }
        new_node->next=before->next;
        before->next=new_node;
        // in case the we are not addinf after the tail.
        if (new_node->next!= nullptr)
            new_node->next->prev=new_node;
        size++;
        return new_node;

    }



//adding a new node with the data and the key before the node after and returning the poiter on the new node in our list
    Node addBefore(Node after,Key key_to_add, Data data_to_add){
        //in this case we  there is no node to add before  it.
        if(after== nullptr)return nullptr;
        //in this case the the node that we need to add befire it is the head of the list then we use the function addBeforeHead.***
        if (after->prev== nullptr) return addBeforeHead(key_to_add,data_to_add);
        //the case that we have a node to add after it
//        if (after->prev) {
        return addAfter(after->prev, key_to_add, data_to_add);
//        }
    }

//adding a new node with the data and the key before the current head in the list and returning the poiter on the new node(new head) in our list

    Node addBeforeHead(Key key_to_add, Data data_to_add){
        //creating the new node to add
        Node new_node = new node(key_to_add,data_to_add);
        //the case that we have an empty list and we are adding the first node to the list
        if(head== nullptr){
            head=new_node;
            tail=new_node;
        } else{
            new_node->next=head;
            head->prev=new_node;
            head=new_node;
        }
        size++;
        return new_node;

    }
    // adding before tail
    Node addAfterTail(Key key_to_add, Data data_to_add){
        //creating the new node to add
        Node new_node = new node(key_to_add,data_to_add);
        //the case that we have an empty list and we are adding the first node to the list
        if(tail== nullptr){
            head=new_node;
            tail=new_node;
        } else{
            new_node->prev=tail;
            tail->next=new_node;
            tail=new_node;
        }
        size++;
        return new_node;

    }

// getting the node if its in our list and returning the pointer on it
    //()
    Node getNode( Node node_to_get){
        Node ite = head;
        while (ite){
            if (ite->key==node_to_get->key)
                return ite;
            ite=ite->next;
        }
        return nullptr;
    }
    Node getNodeByKey( Key key){
        Node ite = head;
        while (ite){
            if (ite->key==key)
                return ite;
            ite=ite->next;
        }
        return nullptr;
    }


    // ()
    void deleteNode(Node to_remove){
        // for thew case of deleting the last node in the list thet means that the list node is the head
        if ( to_remove==head && to_remove->next== nullptr && to_remove){
            delete (to_remove);
            head= nullptr;
            tail= nullptr;
            size=0;
            return;
        }
        //for the case of deleting the head of the list
        if (to_remove&&to_remove==head){
            Node after =to_remove->next;
            head=after;
            after->prev= nullptr;
            size--;
            delete (to_remove);
        }
            //for the case of deleting the tail of the list
        else if (to_remove==tail&&to_remove){
            Node before=to_remove->prev;
            tail=before;
            before->next= nullptr;
            size--;
            delete (to_remove);
        }
            //  for the case of deleting the node in the middle
        else if (to_remove){
            Node before =to_remove->prev;
            Node after =to_remove->next;
            before->next=after;
            after->prev=before;
            delete (to_remove);
            size--;
        }

    }
// (we may change it to deleting the head every time tell we get to an empty list)
//  this funtion destroyes the list by removing each node in it.
    void destroyList(){
        Node ite=head;
        Node temp;
        while (ite){
            temp = ite->next;
            deleteNode(ite);
//            ite=head;
//            ite=ite->next;
            ite=temp;
        }
    }
    //GET FUNCTIONS

    Node getFirst(){
        // updating the itirato to point on the heah node;
        iterator=this->head;
        return iterator;
    }
    Node getLast(){
        return tail;
    }
    Node getNext(){
        iterator= iterator->next;
        return iterator;
    }
    Node getPrev(){
        iterator= iterator->prev;
        return iterator;
    }
    int getSize(){
        return this->size;
    }
    //End of Get functions

    void PrintList(){
        Node itr=getFirst();
        while (itr){
            std::cout<<itr->key<< "->"<<" the data "<<itr->data<<endl;
            itr=getNext();
        }
        std::cout<<std::endl;

    }
};
#endif //WET2_WINTER2020_LINKEDLIST_H
