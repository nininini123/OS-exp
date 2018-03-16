#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <memory.h>
#include <errno.h>
int main() {
    int pid;
    if ( (pid= fork() )  == 0 ) {
        /*** child 1  ***/
        printf("child 1 begins ...\n");
        if ( execlp("../../clock/out/clock","it\'s good to be single",NULL) == -1) {
            printf("M: error execlp \n");
            printf("M: %s \n",strerror(errno));
        }
        printf("i am not supposed to be executed...\n");
    }
    else  if ( (pid= fork() )  == 0 ) {
        /*** child 2  ***/
        printf("child 2 begins ...\n");
        if ( execlp("../../adder/out/adder","it\'s good to be single",NULL) == -1) {
            printf("M: error execlp \n");
            printf("M: %s \n",strerror(errno));
        }
        printf("i am not supposed to be executed...\n");
    }
    else if ( (pid= fork() )  == 0 ) {
        /*** child 3  ***/
        printf("child 3 begins ...\n");
        if ( execlp("../../monitor/out/monitor","it\'s good to be single",NULL) == -1) {
            printf("M: error execlp \n");
            printf("M: %s \n",strerror(errno));
        }
        printf("i am not supposed to be executed...\n");
    }
    else {
        int status;
        wait(&status);
        wait(&status);
        wait(&status);
    }
    return 0;
}