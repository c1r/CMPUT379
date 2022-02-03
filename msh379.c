#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/stat.h>

#include "msh379.h"
#include "commands.h"

struct processes proc[32];
int counter;

void input_p(char* input, clock_t startTime, struct tms *tmsstart);

int main(int argc, char *argv[]){
    //struct processes proc[32];
    counter = 0;

    //----Start recording time----------//
    struct tms tmsstart;
    clock_t startTime = times(&tmsstart);

    
    //----Setting CPU time limit-------//
    struct rlimit rlim;
    getrlimit(RLIMIT_CPU, &rlim);
    rlim.rlim_cur = 600;
    if (setrlimit(RLIMIT_CPU, &rlim) == -1){
       fprintf(stderr, "%s", "Failed to set CPU time limit\n");
    }
    
    while(1){
        printf("msh379 [%d]: ",getpid());
        char input[60];
        // scanf("%s", input);
        fgets(input,60,stdin);
        input[strlen(input)-1]='\0';
        input_p(input, startTime, &tmsstart);
        
        }
        return 0;
    }

