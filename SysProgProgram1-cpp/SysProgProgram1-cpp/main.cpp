/*
 Student Name: Logan Christopher
 Student NetID: clc1085
 Compiler Used: GCC, MacOSX
 Program Description: This program will read strings until “STOP” is entered. Strings will then be displayed in reverse.
 */

#include <iostream>
#include "LinkedList.h"

using namespace std;

int main() {
    int i = 0;
    string buf;
    bool condition = true;
    struct Node *start = NULL;
    
    cout << "This program will read strings until “STOP” is entered\n";
    
    // scan user string and push onto linked list
    while(condition) {
        cout << "Enter string " << i+1 << ": ";
        cin >> buf;
        
        push(&start, buf);
                
        // if user enters "STOP", display strings in reverse and stop program
        if(buf == "STOP") { condition = false; }
        i++;
    }
    
    printf("The strings entered will now be displayed in reverse:\n");
    
    printListReverse(start);
    // free memory allocated by list
    freeList(start);
    
    return 0;
}
