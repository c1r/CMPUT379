/*
 *
 * Name: Nidal Naseem
 * Student ID: 1635297
 * CMPUT 379, Assignment 1 Winter 2022
 */

#ifndef commands_h
#define commands_h
#define MAXWORD 32

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>

// void done(pid_t pid);
int cd(char *pathname);
// void print_umask();
void print_directory();
void run_command(char *command);
void lstasks();
void check_task(char *targetID);
void stop_task(int);
void continue_task(int);
void terminate_task(int);
void terminate_alltasks(clock_t, struct tms *startTime);
void quit_maintask(clock_t, struct tms *startTime);


#endif