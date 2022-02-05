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

void input_p(char *input, clock_t startTime, struct tms *tmsstart)
{
    char array[MAX_NTOKEN][MAXLINE];
    // splitting the input from users into token to seperate commands and arguments
    split(input, array, " ");

    if (strcmp(array[0], "cdir") == 0)
    {
        if (strcmp(array[1], "") == 0)
        {
            printf("Please enter the path to change the current directory to: \n");
        }
        else
        {
            char pathname[sizeof(array[1])];
            strcpy(pathname, array[1]);
            cd(pathname);
        }
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
        if (strcmp(array[1], "") == 0)
        {
            printf("Please enter the pid to check: \n");
        }
        else
        {
            check_task(array[1]);
        }
    }
    else if (strcmp(array[0], "stop") == 0)
    {
        if (strcmp(array[1], "") == 0)
        {
            printf("Please enter the pid of the process to be stopped \n");
        }
        else
        {
            stop_task(atoi(array[1]));
        }
    }
    else if (strcmp(array[0], "continue") == 0)
    {
        if (strcmp(array[1], "") == 0)
        {
            printf("Please enter the pid to be resumed \n");
        }
        else
        {
            continue_task(atoi(array[1]));
        }
    }
    else if (strcmp(array[0], "terminate") == 0)
    {
        if (strcmp(array[1], "") == 0)
        {
            printf("Please enter the pid to be terminated \n");
        }
        else
        {
            terminate_task(atoi(array[1]));
        }
    }
    else if (strcmp(array[0], "exit") == 0)
    {
        terminate_alltasks(startTime, tmsstart);
    }
    else if (strcmp(array[0], "quit") == 0)
    {
        quit_maintask(startTime, tmsstart);
    }
    else if (strcmp(array[0], "run") == 0)
    {
        if (strlen(input) < 4)
        {
            printf("Enter command to be run\n");
        }
        else
        {
            run_command(input);
        }
    }
}

int main(int argc, char *argv[])
{
    // counter for NTASKS
    counter = 0;

    //----Start recording time----------//
    struct tms tmsstart;
    clock_t startTime = times(&tmsstart);

    //----Setting CPU time limit-------//
    struct rlimit rlim;
    getrlimit(RLIMIT_CPU, &rlim);
    rlim.rlim_cur = 600;

    if (startTime == -1)
    {
        printf("Failed to record the end time\n");
    }

    if (setrlimit(RLIMIT_CPU, &rlim) == -1)
    {
        fprintf(stderr, "%s", "Failed to set CPU time limit\n");
    }

    while (1)
    {
        printf("msh379 [%d]: ", getpid());
        char input[60];

        fgets(input, 60, stdin);
        // removing the \n after fgets
        input[strlen(input) - 1] = '\0';
        input_p(input, startTime, &tmsstart);
    }
    return 0;
}
