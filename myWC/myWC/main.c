/*
 Student Name: Logan Christopher
 Student NetID: CLC1085
 Compiler Used: gcc and xcode
 Program Description: This program implements the WC command in C
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct wcRet {
    int l, w, c;
};

               //(change by ref, lwc flags, stream pointer) : returns an object {l, w, c} 
struct wcRet myWC(int *countPtr, int flag, FILE *fp) {
    int l = 0, w = 0, c = 0, BUFFSIZE = 1024;
    char *stdBuf = (char *)malloc(BUFFSIZE*sizeof(char)); // allocate memory for buffer
    if (stdBuf == NULL) {
        perror("Unable to allocate buffer");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(stdBuf, BUFFSIZE, fp) != NULL) {
        int i = 0;
        l += 1;
        while (i <= strlen(stdBuf)+1) {
            if (stdBuf[i] == ' ') {
                w += 1;
            }
            i++;
        }
        w += 1;
        c += strlen(stdBuf);
    }
    
    switch (flag) {
        case 1:
            *countPtr = l;
            break;
            
        case 2:
            *countPtr = w;
            break;
            
        case 3:
            *countPtr = c;
            break;
    }
    struct wcRet ret = {l, w, c};
    return ret;
}

int main(int argc, char *argv[]) {
    int lines = 0, words = 0, chars = 0;
    FILE *fp = 0;
    struct wcRet ret;
    
    if (strcmp(argv[0], "./myWC") == 0) {
        if (argc == 1) {
            ret = myWC(0, 0, stdin);
            lines = ret.l;
            words = ret.w;
            chars = ret.c;
            printf("%d  %d  %d\n", lines, words, chars);
            exit(EXIT_SUCCESS);
        } else if (argc == 2) {
            if (strcmp(argv[1], "-l") == 0) {
                // myWC -l
                myWC(&lines, 1, stdin);
            } else if (strcmp(argv[1], "-w") == 0) {
                // myWC -w
                myWC(&words, 2, stdin);
            } else if (strcmp(argv[1], "-c") == 0) {
                // myWC -c
                myWC(&chars, 3, stdin);
            } else {
                fp = fopen(argv[1], "r");
                if (fp == NULL) {
                    perror("Error opening file\n");
                    exit(EXIT_FAILURE);
                }
                // myWC <filename>
                ret = myWC(0, 0, fp);
                lines = ret.l;
                words = ret.w;
                chars = ret.c;
                fclose(fp);
            }
            printf("%d  %d  %d  %s\n", lines, words, chars, argv[1]);
            exit(EXIT_SUCCESS);
        } else if (argc == 3) {
            fp = fopen(argv[2], "r");
            if (fp == NULL) {
                perror("Error opening file\n");
                exit(EXIT_FAILURE);
            }
            if (strcmp(argv[1], "-l") == 0) {
                // myWC -l <filename>
                myWC(&lines, 1, fp);
            } else if (strcmp(argv[1], "-w") == 0) {
                // myWC -w <filename>
                myWC(&words, 2, fp);
            } else if (strcmp(argv[1], "-c") == 0) {
                // myWC -c <filename>
                myWC(&chars, 3, fp);
            }
            fclose(fp);
            printf("%d  %d  %d  %s\n", lines, words, chars, argv[2]);
            exit(EXIT_SUCCESS);
        } else {
            perror("USAGE: myWC <flag> <filename>\n");
            exit(EXIT_FAILURE);
        }
    }
    
        return 0;
}
