/*
 *
 * Name: Nidal Naseem
 * Student ID: 1635297
 * CMPUT 379, Assignment 1 Winter 2022
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/stat.h>

#include "msh379.h"
#include "commands.h"
#include "util.h"

struct processes proc[32];
int counter;

//void input_p(char *input, clock_t startTime, struct tms *tmsstart);

void input_p(char *input, clock_t startTime, struct tms *tmsstart)
{
    char array[MAX_NTOKEN][MAXLINE];
    // printf("I'm here first lads %s\n",input);
    split(input, array, " ");
    // printf("First token %s",array[1]);
    if (strcmp(array[0], "cdir") == 0)
    {
        char pathname[sizeof(array[1])];
        strcpy(pathname, array[1]);
        cd(pathname);
    }
    else if (strcmp(array[0], "pdir") == 0)
    {
        print_directory();
    }
    else if (strcmp(array[0], "lstasks") == 0)
    {
        lstasks();
    }
    else if (strcmp(array[0], "check") == 0)
    {
        check_task(array[1]);
    }
    else if (strcmp(array[0], "stop") == 0)
    {
        stop_task(atoi(array[1]));
    }
    else if (strcmp(array[0], "continue") == 0)
    {
        continue_task(atoi(array[1]));
    }
    else if (strcmp(array[0], "terminate") == 0)
    {
        terminate_task(atoi(array[1]));
    }
    else if (strcmp(array[0], "exit") == 0)
    {
        terminate_alltasks(startTime, tmsstart);
    }
    else if (strcmp(array[0], "quit") == 0)
    {
        quit_maintask(startTime, tmsstart);
    }
    else if (strcmp(input,"")!=0)
    {
        run_command(input);
    }
}

int main(int argc, char *argv[])
{
    // struct processes proc[32];
    counter = 0;

    //----Start recording time----------//
    struct tms tmsstart;
    clock_t startTime = times(&tmsstart);

    //----Setting CPU time limit-------//
    struct rlimit rlim;
    getrlimit(RLIMIT_CPU, &rlim);
    rlim.rlim_cur = 600;
    if (setrlimit(RLIMIT_CPU, &rlim) == -1)
    {
        fprintf(stderr, "%s", "Failed to set CPU time limit\n");
    }

    while (1)
    {
        printf("msh379 [%d]: ", getpid());
        char input[60];
        // scanf("%s", input);
        fgets(input, 60, stdin);
        input[strlen(input) - 1] = '\0';
        input_p(input, startTime, &tmsstart);
    }
    return 0;
}
