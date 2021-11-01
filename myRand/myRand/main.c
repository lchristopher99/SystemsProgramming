/*
 Student Name: Logan Christopher
 Student NetID: clc1085
 Compiler Used: GCC, MacOSX
 Program Description: This program will generate random numbers 0-100 and write the unioned character bytes to dataX.dat and will return with X.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

union dtob {
    int value;
    unsigned char bytes[10];
};

int myRand(void) {
    char filename[12];
    union dtob outBytes;
    
    srand((unsigned)time(NULL));
    int X = rand() % (255 + 1);
    sprintf(filename, "data%d.dat", X);
    
    printf("Opening '%s' to write...\n", filename);
    
    FILE *fp = fopen(filename, "wb");

    if (fp == NULL) {
        printf("Error opening file...\n");
        return -1;
    }
    
    for (int i = 0; i < 60; i++) {
        outBytes.value = rand() % (100 + 1);
        fwrite(outBytes.bytes, sizeof(outBytes.bytes), 1, fp);
        printf("#%d: %d, ", i+1, outBytes.value);
    }
    
    fclose(fp);
    return X;
}

int main(int argc, const char *argv[]) {
    int X = myRand();
    if (X == -1) {
        printf("Error in myRand...\n");
        exit(EXIT_FAILURE);
    } else {
        exit(X);
    }
}
