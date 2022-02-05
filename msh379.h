/*
 *
 * Name: Nidal Naseem
 * Student ID: 1635297
 * CMPUT 379, Assignment 1 Winter 2022
 */

#ifndef msh379_h
#define msh379_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <signal.h>
#include <sys/wait.h>

// GLOBAL struct that keeps tracks of the information of all accepted tasks
struct processes
{
    pid_t pid;
    char command[60];
    int index;
};
extern struct processes proc[32];
extern int counter;

void input_p(char *input, clock_t startTime, struct tms *tmsstart);

#endif
