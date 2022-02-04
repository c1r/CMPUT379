/*
 *
 * Name: Nidal Naseem
 * Student ID: 1635297
 * CMPUT 379, Assignment 1 Winter 2022
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>

#define MAXLINE 260
#define MAX_NTOKEN MAXLINE
#define MAXWORD 32

int split(char inStr[], char token[MAX_NTOKEN][MAXLINE], char fs[])
{
    int i, count;
    char *tokenp, inStrCopy[MAXLINE];

    count = 0;
    memset(inStrCopy, 0, sizeof(inStrCopy));
    for (i = 0; i < MAXLINE; i++)
        memset(token[i], 0, sizeof(token[i]));

    strcpy(inStrCopy, inStr);
    if ((tokenp = strtok(inStr, fs)) == NULL)
        return (0);

    strcpy(token[count], tokenp);
    count++;

    while ((tokenp = strtok(NULL, fs)) != NULL)
    {
        strcpy(token[count], tokenp);
        count++;
    }
    strcpy(inStr, inStrCopy);
    return (count);
}

void print_times(clock_t realTime, struct tms *tmsstart, struct tms *tmsend)
{
    long clktck = 0;

    if (clktck == 0)
    {
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
        {
            printf("Failed to fetch clock ticks per second\n");
        }
    }

    printf("   real:%12.2f\n", realTime / (double)clktck);

    printf("   user:%12.2f\n",
           (tmsend->tms_utime - tmsstart->tms_utime) / (double)clktck);

    printf("   sys:%13.2f\n",
           (tmsend->tms_stime - tmsstart->tms_stime) / (double)clktck);

    printf("   child user:%6.2f\n",
           (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);

    printf("   child sys:%7.2f\n",
           (tmsend->tms_cstime - tmsstart->tms_cstime) / (double)clktck);
}