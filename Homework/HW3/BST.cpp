#include <stdio.h>
#include <iostream>

class Node
{
public:
    int data;
    Node *left;
    Node *right;
    
    
    //constructor 
    Node(int value){
        data =value;
        left = NULL;
        right = NULL;
    }
    
    void  print() const {
        
        std::cout <<"("<< data << ")\n";
    }
};



int main()
{
    Node *root = new Node(1);
    root->left = new Node(98);
    root->right = new Node(3);
    
    root->left->print();
    
    
}
