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

//void done(pid_t pid);
int cd(char* pathname);
//void print_umask();
void print_directory();
void run_command(char* command);
void lstasks();
void check_task(int);
void stop_task(int);
void continue_task(int);
void terminate(int);
void terminate_alltasks(clock_t, struct rlimit *rlim);
void quit_maintask(clock_t, struct rlimit *rlim);
//void print_pr_times(clock_t real_time, struct tms *tmsstart, struct tms *tmsend);
int split(char inStr[], char token[][MAXWORD], char fs[]);


#endif