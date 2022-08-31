//
//  RankAVL.h
//  WET-2-DS
//
//  Created by Amr Dacca on 13/06/2021.
//

#ifndef RankAVL_h
#define RankAVL_h

#include <iostream>

template<class Key,class Data>
class RankAVL{
public:
    typedef class node{
    public:
        Key key;
        Data data;
        node* left;
        node* right;
        node* parent;
        int height;
        int rank;
        Data sum;
        node(Key key,Data data,node* parent): key(key),data(data),left(nullptr),right(nullptr),parent(parent),height(0),rank(1),sum(data){};
        ~node()=default;


    }*Node;
public:
    int size;
    Node root;
    Node min_node;
    Node max_node;
//**------------------------------------------------------------------------------------------------------------**
    RankAVL():size(0),root(nullptr),min_node(nullptr),max_node(nullptr){};
    RankAVL(const RankAVL &other_tree);
    RankAVL& operator=(const RankAVL other_tree);
    void copyTree(Node root);

    ~RankAVL(){
        deleteAVL();
    }
    void deleteAVL(){
        destroyAVL(root);
        root=nullptr;
        min_node=nullptr;
        max_node=nullptr;
        size=0;
    }
    static void destroyAVL(Node root){
        if (root==nullptr) {
            return;
        }
        destroyAVL(root->left);
        destroyAVL(root->right);
        delete (root);
    }

    void treeClear(){
        destroyAVL(root);
        size=0;
        root=nullptr;
        min_node=nullptr;
        max_node=nullptr;
    }
    Node findNode(Key key){
        return containNode(this->root, key);;
    }
    bool containsNode(Key key){
        if (containNode(this->root,key)) {
            return true;
        }
        return false;
    }
    static Node containNode(Node node,Key key){
        if(node==nullptr){
            return nullptr;
        }
        if(key == node->key){
            return node;
        }
        if (key > node->key) {
            return containNode(node->right,key);
        }
        return containNode(node->left,key);

    }

    bool insertNode(Key key,Data data){
        Node new_node = new node(key,data,nullptr);
        if(containsNode(key)){
            delete new_node;
            return false;
        }
        root=addNode(root, new_node);
        root->parent=nullptr;
        size++;
        updateHeight(root);
        updateWeight(root);
        updateSum(root);
        updateMin();
        updateMax();
        return true;

    }
    Node addNode(Node r ,Node new_node){
        if (r==nullptr){
            new_node->parent = r;
            return new_node;
        }
        else if(new_node->key > r->key){
            r->right =addNode(r->right, new_node);
            r->right->parent=r;
        }
        else if(new_node->key < r->key){
            r->left =addNode(r->left, new_node);
            r->left->parent=r;
        }
        updateHeight(r);
        updateWeight(r);
        updateSum(r);
        return balanceAVL(r);

    }
    bool removeNode(Key key){

        if (containsNode(key)==false) {
            return false;
        }
        root=deleteNode(root, key);
        size--;
        updateHeight(root);
        updateWeight(root);
        updateSum(root);
        updateMin();
        updateMax();
        return true;
    }

    Node deleteNode(Node r,Key key){
        if(key > r->key){
            r->right= deleteNode(r->right, key);
        }
        else if(key < r->key){
            r->left= deleteNode(r->left, key);
        }
        else if(key == r->key){
            ///node is a leaf
            if(r->left==nullptr && r->right == nullptr){
                Node tmp = r;
                r=r->parent;
                delete tmp;
                return nullptr;
            }
            else if(r->left!=nullptr && r->right == nullptr){
                Node tmp = r->left;
                tmp->parent = r->parent;
                delete r;
                return tmp;

            }
            else if(r->left==nullptr && r->right != nullptr){
                Node tmp = r->right;
                tmp->parent = r->parent;
                delete r;
                return tmp;
            }
            else{
                Node success = findSuccessorHelper(r);
                r->key = success->key;
                r->data = success->data;
                r->right = deleteNode(r->right, success->key);

            }
        }
        updateHeight(r);
        updateWeight(r);
        updateSum(r);
        return balanceAVL(r);

    }

    static int getHeight(Node node){
        if(node==nullptr)
            return -1;
        return node->height;
    }
    static int getWeight(Node node){
        if(node==nullptr)
            return 0;
        return node->rank;
    }
    static Data getSum(Node node){
        if (node == nullptr) {
            return 0;
        }
        return node->sum;
    }
    static void updateSum(Node node){
        if(node){
            Data left_sum=getSum(node->left);
            Data right_sum=getSum(node->right);
            node->sum=left_sum+right_sum+node->data;
        }
    }
    static void updateWeight(Node node){
        if(node){
            int left_rank=getWeight(node->left);
            int right_rank=getWeight(node->right);
            node->rank=left_rank+right_rank+1;
        }
    }
    int rank(Key key){
        if (!containsNode(key)) {
            return -1;
        }
        return rank(root,key);
    }
    static int rank(Node r,Key key){
        int count=0;
        if(r->key > key){
            count +=rank(r->left, key);
        }
        else if(r->key < key ){
            count += getWeight(r->left)+ 1 +rank(r->right, key);
        }
        else{
            return count+=getWeight(r->left)+ 1;
        }
        return count;
    }
    Node select(int k) {
        if (k < 0){
            return nullptr;
        }
        return select(root, k);
    }
    static Node select(Node node, int k) {//STARTS WITH 1

        if (node == nullptr)return node;
        if (getWeight(node->left) == k-1)return node;
        if (getWeight(node->left) > k-1 )return select(node->left, k);

        return select(node->right,k - getWeight(node->left)-1);
    }
    static void updateHeight(Node node){
        if(node){
            int left_height=getHeight(node->left);
            int right_height=getHeight(node->right);
            if(left_height>right_height)
                node->height=1+left_height;
            else if (right_height>left_height){
                node->height=1+right_height;
            }
            else {
                node->height=1+right_height;
            }
        }
    }

    static int calcBF(Node node){///COULD MAKE PROBLEMS IN RECURSIVE
        int left_height,right_height;
        if (node->left!=nullptr) {
            left_height=getHeight(node->left);
        }
        else{
            left_height=-1;
        }
        if (node->right==nullptr) {
            right_height=-1;
        }
        else{
            right_height=getHeight(node->right);
        }
        return left_height-right_height;

    }
    void updateMax(){
        if(root == nullptr){
            max_node = nullptr;
            return;
        }
        Node itr=root;
        while (itr->right) {
            itr=itr->right;
        }
        max_node=itr;
    }
    void updateMin(){
        if(root == nullptr){
            min_node = nullptr;
            return;
        }
        Node itr=root;
        while (itr->left) {
            itr=itr->left;
        }
        min_node=itr;
    }
    typedef enum rotation {
        LL = 0,
        LR = 1,
        RL = 2,
        RR = 3,
        NONE = 4
    } RotationChoice;

    static Node findSuccessorHelper(Node node){
        if (node == nullptr) {
            return nullptr;
        }
        if(node->right == nullptr){
            Node itr = node->parent;
            Node tmp = node;
            while (itr && itr->right == tmp) {
                tmp=itr;
                itr=itr->parent;
            }
            return itr;
        }
        Node tmp_next = node->right;
        while (tmp_next->left) {
            tmp_next= tmp_next->left;
        }
        return tmp_next;
    }

    RotationChoice chooseRotation(Node node){
        int BF_node= calcBF(node);
        int BF_node_left= 0;
        int BF_node_right= 0;
        if (node->right) {
            BF_node_right=calcBF(node->right);
        }
        if (node->left) {
            BF_node_left=calcBF(node->left);
        }
        switch (BF_node) {
            case 2:{
                if(BF_node_left>=0)
                    return LL;
                if(BF_node_left==-1)
                    return LR;
                break;
            }
            case -2:{
                if(BF_node_right<=0)
                    return RR;
                if(BF_node_right==1)
                    return RL;
                break;
            }
            default:
                return NONE;
                break;
        }
        return NONE;
    }

    Node balanceAVL(Node node){
        RotationChoice choice = chooseRotation(node);
        switch (choice) {
            case LL:
                return LLRotation(node);
            case RR:
                return RRRotation(node);
            case RL:
                return RLRotation(node);
            case LR:
                return LRRotation(node);
            default:
                return  node;
        }
    }

    Node rotateRight(Node B){
        Node tmpB_parent = B->parent;
        Node A = B->left;
        B->left = A->right;
        A->right = B;
        A -> parent = tmpB_parent;
        B->parent = A;
        if(B->left !=nullptr){
            B->left->parent = B;
        }
        updateHeight(B);
        updateWeight(B);
        updateSum(B);
        updateHeight(A);
        updateWeight(A);
        updateSum(A);
        return A;
    }
    Node rotateLeft(Node B){
        Node tmpB_parent = B->parent;
        Node A = B->right;
        B->right = A->left;
        A->left = B;
        A->parent= tmpB_parent;
        B->parent=A;
        if(B->right !=nullptr){
            B->right->parent = B;
        }
        updateHeight(B);
        updateWeight(B);
        updateSum(B);
        updateHeight(A);
        updateWeight(A);
        updateSum(A);
        return A;


    }
    Node LLRotation(Node node){
        return rotateRight(node);
    }
    Node RRRotation(Node node){
        return rotateLeft(node);
    }
    Node LRRotation(Node node){
        node->left =rotateLeft(node->left);
        return rotateRight(node);
    }
    Node RLRotation(Node node){
        node->right =rotateRight(node->right);
        return rotateLeft(node);
    }
    static Node sortedArrayToAVL(Key arr[],Data data[],int start, int end,Node parent){
        /* Base Case */
        if (start > end)
            return nullptr;

        /* Get the middle element and make it root */
        int mid = (start + end)/2;
        Node root = new node(arr[mid],data[mid],parent);
        /* Recursively construct the left subtree
        and make it left child of root */
        root->left = sortedArrayToAVL(arr,data ,start,mid - 1,root);

        /* Recursively construct the right subtree
        and make it right child of root */
        root->right = sortedArrayToAVL(arr,data, mid + 1, end,root);
        return root;
    }
    void updateAllNodesHeight(Node node)
    {
        if (node == nullptr)
            return;
        updateAllNodesHeight(node->left);
        updateAllNodesHeight(node->right);
        updateHeight(node);
        updateWeight(node);
        updateSum(node);
    }
    static void RankAVLtoArray(Key* arr,Data* arr2,Node node){//CALL WITH MIN
        int start=0;
        if (node==nullptr) {
            return;
        }
        do {
            arr[start]=node->key;
            arr2[start]=node->data;
            start++;
            node=findSuccessorHelper(node);
        }while(node);
    }
};

#endif /* RankAVL_h */
