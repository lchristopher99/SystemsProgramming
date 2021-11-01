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
#include <sys/time.h>
#include <time.h>

static pid_t parentPID, childPID;
static FILE *fp;
union btod inBytes;
int count = 1;
time_t start, end;

union btod {
    int value;
    unsigned char bytes[10];
};

static void handle_signal(int sig) {
    if (sig == SIGCHLD) {
        printf("Warning! Pressure control monitoring has been terminated, exiting the system\n");
        while (waitpid(-1, NULL, WNOHANG) > 0) continue;
        exit(EXIT_SUCCESS);
    }
    if (sig == SIGINT) {
        if (getpid() == parentPID) {
            char *buf = "  EXIT: Are you sure? (Y/N): ";
            char ans[1];
            write(STDOUT_FILENO, buf, strlen(buf));
            read(STDIN_FILENO, ans, 1);
            if (ans[0] == 'Y' || ans[0] == 'y') {
                kill(childPID, SIGCHLD);
                exit(EXIT_SUCCESS);
            }
        }
    }
    if (sig == SIGUSR1) {
        end = time(NULL);
        if (start > 0) {
            printf("Warning! Line pressure is dangerously low. It has been %ld seconds since the previous warning\n", end-start);
        } else { printf("Warning! Line pressure is dangerously low. It has been 0 seconds since the previous warning\n"); }
        start = time(NULL);
    }
    if (sig == SIGUSR2) {
        end = time(NULL);
        if (start > 0) {
            printf("Warning! Line pressure is dangerously high. It has been %ld seconds since the previous warning\n", end-start);
        } else { printf("Warning! Line pressure is dangerously high. It has been 0 seconds since the previous warning\n"); }
        start = time(NULL);
    }
    if (sig == SIGALRM) { // process and send usr1 or usr2 signal back to parent
        if (fread(inBytes.bytes, sizeof(inBytes.bytes), 1, fp) != 0) {
            printf("%d: %d\n", count++, inBytes.value);
            if (inBytes.value >= 450 && inBytes.value <= 550) {
                kill(parentPID, SIGUSR1);
            } else if (inBytes.value >= 1450 && inBytes.value <= 1550) {
                kill(parentPID, SIGUSR2);
            }
        } else {
            kill(parentPID, SIGCHLD);
            exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, const char * argv[]) {
    struct sigaction sa;
    struct itimerval itmr;
    
    // open file
    printf("Opening 'flowData.dat' to read...\n");
    fp = fopen("flowData.dat", "rb");

    if (fp == NULL) {
        printf("Error opening file...\n");
        exit(EXIT_FAILURE);
    }
    
    // set interval
    itmr.it_interval.tv_sec = 5;
    itmr.it_interval.tv_usec = 0;
    itmr.it_value.tv_sec = 3;
    itmr.it_value.tv_usec = 0;
    
    // clear set and create user sighandler
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction for SIGINT");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction for SIGCHLD");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction for SIGUSR1");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction for SIGUSR2");
        exit(EXIT_FAILURE);
    }

    parentPID = getpid();
    switch (childPID = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
            
        case 0: // child
            if (sigaction(SIGALRM, &sa, NULL) == -1) {
                perror("sigaction for SIGALRM");
                exit(EXIT_FAILURE);
            }
            if (setitimer(ITIMER_REAL, &itmr, NULL) == -1) {
                perror("setitimer");
                exit(EXIT_FAILURE);
            }
            while (1) {
                pause();
            }
            
        default: // parent
            while (1) {
                char buf[1024];
                if (fgets(buf, 1024, stdin) != NULL) {
                      puts(buf);
                }
            }
    }
}
