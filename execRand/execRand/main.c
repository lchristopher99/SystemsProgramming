//
//  main.c
//  execRand
//
//  Created by Logan Christopher on 10/5/21.
//

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

union btod {
    int value;
    unsigned char bytes[10];
};

int readFile(char *filename) {
    FILE *fp;
    union btod inBytes;
    int avg = 0;

    printf("Opening '%s' to read...\n", filename);
    fp = fopen(filename, "rb");

    if (fp == NULL) {
        printf("Error opening file...\n");
        return -1;
    }
    
    int i = 0;
    while (fread(inBytes.bytes, sizeof(inBytes.bytes), 1, fp) == 1) {
        printf("#%d: %d, ", i+1, inBytes.value);
        avg += inBytes.value;
    }
    avg /= 60;
    printf("\nAverage of all 60 values: %d\n", avg);

    fclose(fp);
    // "4. unlink the data file." ?
    if (remove(filename) == 0) {
        printf("\nFile: '%s' deleted successfully...\n", filename);
    } else {
        printf("\nProblem deleting '%s'...\n", filename);
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    static pid_t childPID;
    char filename[12];
    int status;
    
    switch (childPID = fork()) {
        case 0: // child
            execv("myRand", NULL);
            break;
            
        default: // parent
            while (1) {
                waitpid(childPID, &status, 0); // wait for child to finish
                if (WIFEXITED(status)) {
                    status = WEXITSTATUS(status);
                    printf("\nChild process finished with status: %d...\n\n", status);
                    sprintf(filename, "data%d.dat", status);
                    if (readFile(filename) == -1) {
                        perror("Error reading file...\n");
                        exit(EXIT_FAILURE);
                    } else {
                        printf("\nParent process finished...\n\n");
                        exit(EXIT_SUCCESS);
                    }
                }
            }
    }
}
