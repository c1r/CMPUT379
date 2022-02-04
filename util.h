/*
 *
 * Name: Nidal Naseem
 * Student ID: 1635297
 * CMPUT 379, Assignment 1 Winter 2022
 */


#ifndef util_h
#define util_h

#define MAXWORD 32
#define MAXLINE 260
#define MAX_NTOKEN MAXLINE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>

void print_times(clock_t real_time, struct tms *tmsstart, struct tms *tmsend);
int split(char inStr[], char token[][MAXLINE], char fs[]);

#endif
