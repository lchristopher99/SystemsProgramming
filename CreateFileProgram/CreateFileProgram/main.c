/*
 Student Name: Logan Christopher
 Student NetID: clc1085
 Compiler Used: GCC, MacOSX
 Program Description: This program will generate random numbers within a user-specified range and output the unioned character buffer to a user-specified out-file as well as the console.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// union for char buffer
union dtob {
    int value;
    unsigned char bytes[10];
};

int createFile(char *fileName, int number, int lVal, int mVal, int hVal) {
    int range, i, alertCount = 0, alert = 0;
    float offset;
    union dtob outBytes;
    
    printf("Opening '%s' to write...\n", fileName);
    
    FILE *fp = fopen(fileName, "wb");

    if (fp == NULL) {
        printf("Error opening file...\n");
        return -1;
    }
    
    for (i = 0; i < number; i++) {
        range = rand() % 100;
        offset = (rand() % 1000 - 500)/1000.0;
        
        if (range < 15) {
            outBytes.value = ((lVal + offset) * 100);
        } else if (range > 85) {
            outBytes.value = ((hVal + offset) * 100);
        } else {
            outBytes.value = ((mVal + offset) * 100);
        }
        
        fwrite(outBytes.bytes, sizeof(outBytes.bytes), 1, fp);
        printf("#%d: %d\n", i+1, outBytes.value);
        
        // report invalid flow
        if (outBytes.value >= 950 && outBytes.value <= 1050) {
            alertCount += 1;
        } else {
            if (outBytes.value >= 450 && outBytes.value <= 550) {
                printf("\nPressure in lines is 'low'.\n");
                alert = 1;
            } else if (outBytes.value >= 1450 && outBytes.value <= 1550) {
                printf("\nPressure in lines is 'high'\n");
                alert = 1;
            }
            if (alert) { printf("Data entries read since last alert: %d\n", alertCount+1); }
            alertCount = 0;
            alert = 0;
        }
    }
    
    fclose(fp);
    return 0;
}

int main(int argc, const char *argv[]) {
    char fileName[1024];
    int number, lVal, mVal, hVal;
    
    if (strcmp(argv[0], "./createFile") == 0) {
        if (argc == 1) { // ./createFile
            strcpy(fileName, "flowData.dat");
            number = 60;
            lVal = 5;
            mVal = 10;
            hVal = 15;
        } else if (argc == 3) { // ./createFile <filename> <num>
            strcpy(fileName, argv[1]);
            number = atoi(argv[2]);
            lVal = 5;
            mVal = 10;
            hVal = 15;
        } else { // ./createFile <filename> <num> <lval> <mval> <hval>
            strcpy(fileName, argv[1]);
            number = atoi(argv[2]);
            lVal = atoi(argv[3]);
            mVal = atoi(argv[4]);
            hVal = atoi(argv[5]);
        }
        if (createFile(fileName, number, lVal, mVal, hVal) == -1) {
            printf("error calling createFile");
            exit(EXIT_FAILURE);
        }
    }
    
    exit(EXIT_SUCCESS);
}
