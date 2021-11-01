/*
 Student Name: Logan Christopher
 Student NetID: clc1085
 Compiler Used: GCC, MacOSX
 Program Description: This program creates a directory structure for all of your files associated with the exercises, assignments, and tests.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void makeDir(char *rootName, char *path, int count) {
    mode_t filePerms = S_IRWXU | S_IRWXG | S_IRWXO;
    char tempPath[1024], childPath[1024], tmp[1024];
    int i = 0;
    
    // append '/path' to rootName
    strcpy(childPath, path); // childPath = /path
    strcpy(tempPath, rootName); // tempPath = rootName
    strcat(tempPath, childPath); // tempPath = rootName/path
    
    // create rootName/path
    if (mkdir(tempPath, filePerms) == -1) {
        perror("error creating test directory\n");
        exit(EXIT_FAILURE);
    }
    
    while (i != count) {
        strcpy(tmp, tempPath); // tmp = rootName/path
        strcat(tmp, path); // tmp = rootName/path/path
        sprintf(childPath, "%d", i+1); // childPath = N
        strcat(tmp, childPath); // tmp = rootName/path/pathN
        if (mkdir(tmp, filePerms) == -1) {
            perror("error creating test sub-directory\n");
            exit(EXIT_FAILURE);
        }
        printf("%s------done\n", tmp);
        i++;
    }
    printf("%s created successfully!\n\n", tempPath);
}

int setupClass(char *rootName) {
    // pass rootName, sub-dir path, and number of children per path
    makeDir(rootName, "", 0);
    makeDir(rootName, "/test", 3);
    makeDir(rootName, "/program", 5);
    makeDir(rootName, "/exercise", 10);
    
    return 1;
};

int main(int argc, const char * argv[]) {
    char *rootName = "sysProg";
    
    // if setupClass is called with 1 arg, set to root name
    if (strcmp(argv[0], "./setupClass") == 0) {
        if (argc == 2) {
            rootName = argv[1];
        }
        if (setupClass(rootName) == 1) {
            printf("%s directory framework succesfully created!\n", rootName);
        }
    }
    
    return 0;
}
