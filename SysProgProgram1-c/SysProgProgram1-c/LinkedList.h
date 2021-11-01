/*
 Student Name: Logan Christopher
 Student NetID: clc1085
 Compiler Used: GCC, MacOSX
 Program Description: header file for linked list
 */

#ifndef LinkedList_h
#define LinkedList_h

struct Node {
    char *data;
    struct Node *next;
};

void push(struct Node** head, void *newData, size_t dataSize) {
    // create new temp node
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
      
    // allocate space for new data and reference head of list
    newNode->data = malloc(dataSize);
    newNode->next = *head;
      
    // copy the new data to node in list
    int i;
    for (i=0; i<dataSize; i++) {
        *(char *)(newNode->data + i) = *(char *)(newData + i);
    }
    
    // re-reference head to the new node being pushed
    *head = newNode;
}

// print list normally
void printList(struct Node *node) {
    while (node != NULL) {
        printf("%s\n", node->data);
        node = node->next;
    }
}

// print strings reversed
void printListReverse(struct Node *node) {
    while (node != NULL) {
        int len = strlen(node->data)-1;
        // create temp char starting at index 0 of string
        // set first char of node->data to last char of node->data
        // set last char of node->data to temp char
        // continue process thru char array while decreasing len
        for(int i=0; i<len; i++) {
           char tmp = node->data[i];
           node->data[i] = node->data[len];
           node->data[len] = tmp;
           len--;
        }
         
        printf("%s\n", node->data);
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
