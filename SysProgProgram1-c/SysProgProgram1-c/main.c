/*
 Student Name: Logan Christopher
 Student NetID: clc1085
 Compiler Used: GCC, MacOSX
 Program Description: This program will read strings until “STOP” is entered. Strings will then be displayed in reverse.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedList.h"

int main() {
    int i = 0;
    char buf[1024];
    bool condition = true;
    struct Node *start = NULL;
    
    printf("This program will read strings until “STOP” is entered\n");
    
    // scan user string and push onto linked list
    while(condition) {
        printf("Enter string %d: ", i+1);
        scanf("%s", buf);
        
        unsigned charSize = sizeof(char) * (strlen(buf) + 1);
        push(&start, buf, charSize);
                
        // if user enters "STOP", display strings in reverse and stop program
        if(strcmp(buf,"STOP") == 0) { condition = false; }
        i++;
    }
    
    printf("The strings entered will now be displayed in reverse:\n");
    
    printListReverse(start);
    // free memory allocated by list
    freeList(start);
    
    return 0;
}
