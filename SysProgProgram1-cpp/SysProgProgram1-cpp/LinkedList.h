/*
 Student Name: Logan Christopher
 Student NetID: clc1085
 Compiler Used: GCC, MacOSX
 Program Description: header file for linked list
 */

#ifndef LinkedList_h
#define LinkedList_h

#include <iostream>

using namespace std;

struct Node {
    string data;
    Node *next;
};

void push(Node** head, string newData) {
    // create new temp node
    Node* newNode = new Node;
      
    // push new data to node and reference head of list
    newNode->data = newData;
    newNode->next = *head;
    
    // re-reference head to the new node being pushed
    *head = newNode;
}

// print list normally
void printList(Node *node) {
    while (node != NULL) {
        cout << node->data;
        node = node->next;
    }
}

// print strings reversed
void printListReverse(Node *node) {
    while (node != NULL) {
        string tmp = node->data;
        reverse(tmp.begin(), tmp.end());
        cout << tmp + "\n";
        node = node->next;
    }
}

// loop thru list and free allocated space
void freeList(struct Node* head) {
   struct Node* tmp;

   while (head != NULL) {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

#endif /* LinkedList_h */
