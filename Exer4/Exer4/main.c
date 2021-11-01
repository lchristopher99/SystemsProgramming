/*
 Student Name: Logan Christopher
 Student NetID: CLC1085
 Compiler Used: gcc and xcode
 Program Description: This program creates a child process; the parent and the child perform different tasks.
*/

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

static pid_t parentPid, childPid;
static int x, y;

static void handle_signal(int sig) {
    if (sig == SIGCHLD) {
        printf("Child exited. Cleaning up...\n");
        while (waitpid(-1, NULL, WNOHANG) > 0) continue;
        exit(EXIT_SUCCESS);
    }
    if (sig == SIGINT) {
        if (getpid() == parentPid) {
            char *buf = "  EXIT: Are you sure? (Y/N): ";
            char ans[1];
            write(STDOUT_FILENO, buf, strlen(buf));
            read(STDIN_FILENO, ans, 1);
            if (ans[0] == 'Y' || ans[0] == 'y') {
                kill(childPid, SIGCHLD);
                exit(EXIT_SUCCESS);
            }
        }
    }
    if (sig == SIGUSR1) {
        x++;
        printf("The child has generated %d values less than 25.\n", x);
    }
    if (sig == SIGUSR2) {
        y++;
        printf("The child has generated %d values greater than 75.\n", y);
    }
    if (sig == SIGALRM) { // process and send usr1 or usr2 signal back to parent
        int num = rand() % (100 + 1 - 0);
        printf("%d\n", num);
        if (num < 25) {
            kill(parentPid, SIGUSR1);
        } else if (num > 75) {
            kill(parentPid, SIGUSR2);
        }
    }
}

int main(int argc, const char * argv[]) {
    if (strcmp(argv[0], "./exer4") == 0) {
        struct sigaction sa;
        struct itimerval itmr;
                
        // set interval to 15 sec
        itmr.it_interval.tv_sec = 5;
        itmr.it_interval.tv_usec = 0;
        itmr.it_value.tv_sec = 3;
        itmr.it_value.tv_usec = 0;
        
        // clear set and create user sighandler
        sigemptyset(&sa.sa_mask);
        sa.sa_handler = handle_signal;
        sa.sa_flags = 0;
        
        parentPid = getpid();
        srand((unsigned)time(NULL));
        
        // create parent signals
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
        
        // spawn child
        switch (childPid = fork()) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
                
            case 0: // create child signals
                if (sigaction(SIGALRM, &sa, NULL) == -1) {
                    perror("sigaction for SIGALRM");
                    exit(EXIT_FAILURE);
                }
                if (setitimer(ITIMER_REAL, &itmr, NULL) == -1) {
                    perror("setitimer");
                    exit(EXIT_FAILURE);
                }
                while (1) {
                    pause(); // waits for timer to throw SIGALRM after interval
                }
                break;
                
            default: // parent
                while (1) {
                    pause(); // waits for sigint, sigchld, usr1, and usr2 signals from child
                }
        }
        
    } else {
        perror("USAGE: ./exer4\n");
        exit(EXIT_FAILURE);
    }
}
