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
#include "msh379.h"
#include "util.h"

#define MAXLINE 260
#define MAX_NTOKEN MAXLINE
#define MAXWORD 32

int cd(char *pathname);
void print_directory();
void run_command(char *command);
void lstasks();
void check_task(char *targetPID);
void stop_task(int);
void continue_task(int);
void terminate_task(int);
void terminate_alltasks(clock_t, struct tms *tmsstart);
void quit_maintask(clock_t, struct tms *tmsstart);

int cd(char *pathname)
{
    char final_directory[MAXLINE];
    // printf("%s Im here lads", pathname);

    if (pathname[0] == '$')
    {
        memmove(pathname, pathname + 1, strlen(pathname));

        // To obtain env variable expansion and thus its directory
        char *env_varexp = strtok(pathname, "/\n");
        char *env_var_dir = getenv(env_varexp);
        if (env_var_dir == NULL)
        {
            printf("cd: no such environment variable: %s\n", env_varexp);
            return -1;
        }

        // Copying to final
        strcpy(final_directory, env_var_dir);

        // To obtain the directory after the env_var

        char *delim = "";
        char *rest = strtok(NULL, delim);

        if (*rest != '\0')
        {
            // adding a '/' before appending the rest of the pathname directory
            strcat(final_directory, "/");
            strcat(final_directory, rest);
        }
    }

    else
    {
        // Just the env_var pathname is copied to final
        strcpy(final_directory, pathname);
    }

    if (chdir(final_directory) != 0)
    {
        printf("cdir: failed (pathname= %s):\n", final_directory);
        return -1;
    }
    printf("cdir: done (pathname= %s)\n", final_directory);
    return 1;
}

void lstasks()
{
    // printf("counter: %d", counter);
    for (int i = 0; i < counter; i++)
    {
        if (strcmp(proc[i].command, "") != 0)
        {
            printf("%d: (pid: %d, cmd: %s)\n", proc[i].index, proc[i].pid, proc[i].command);
        }
    }
}

void check_task(char *targetID)
{
    FILE *fp;
    int status = 0;
    char line[MAXLINE];
    char buffer[MAXLINE];
    char tokens[MAX_NTOKEN][MAXLINE];
    int listPID[32];

    int numOfPID = 0;
    int targetPID = atoi(targetID);

    fp = popen("ps -u $USER -o user,pid,ppid,state,start,cmd --sort start", "r");
    if (fp)
    {
        printf("target_pid = %d\n",targetPID);
        while (fgets(line, MAXLINE, fp) != NULL)
        {
            if (strstr(line, "PID"))
            {
                printf("%s", line);
            }
            strcpy(buffer, line);

            split(buffer, tokens, " ");
            int pid = atoi(tokens[1]);
            int ppid = atoi(tokens[2]);
            char *state = tokens[3];

            if (pid == targetPID)
            {
                printf("%s", line);
                // printf("target_pid = %d running:\n",pid);
                if (strcmp(state, "Z") == 0)
                {

                    break;
                    listPID[numOfPID] = pid;
                    numOfPID++;
                }
            }

            if (ppid == targetPID)
            {
                printf("%s", line);

                listPID[numOfPID] = pid;
                numOfPID++;
            }

            for (int i = 0; i < numOfPID; i++)
            {
                if (listPID[i] == ppid)
                {
                    printf("%s", line);

                    listPID[numOfPID] = pid;
                    numOfPID++;
                }
            }
        }
    }

    status = pclose(fp);
    if (status == -1)
        printf("Close failed\n");
}

void stop_task(int task)
{
    if (proc[task].index == task)
    {
        kill(proc[task].pid, SIGSTOP);
    }
    else
    {
        printf("Couldn't find task with index: %d", task);
    }
}

void continue_task(int task)
{
    if (proc[task].index == task)
    {
        kill(proc[task].pid, SIGCONT);
    }
    else
    {
        printf("Couldn't find task with index: %d", task);
    }
}

void terminate_task(int task)
{
    if (proc[task].index == task && strcmp(proc[task].command, "") != 0)
    {
        printf("%d: (pid: %d, cmd: %s) terminated\n", proc[task].index, proc[task].pid, proc[task].command);

        kill(proc[task].pid, SIGCONT);
        kill(proc[task].pid, SIGTERM);
        kill(proc[task].pid, SIGKILL);
        proc[task].index = '\0';
        proc[task].pid = '\0';
        strcpy(proc[task].command, "\0");
    }
}

void terminate_alltasks(clock_t startTime, struct tms *tmsstart)
{
    for (int i = 0; i < counter; i++)
    {
        terminate_task(i);
    }

    struct tms tmsend;
    clock_t endTime = times(&tmsend);
    if (endTime == -1)
    {
        printf("Failed to record the end time\n");
    }
    print_times(endTime - startTime, tmsstart, &tmsend);
}

void quit_maintask(clock_t startTime, struct tms *tmsstart)
{

    struct tms tmsend;
    clock_t endTime = times(&tmsend);
    if (endTime == -1)
    {
        printf("Failed to record the end time\n");
    }
    print_times(endTime - startTime, tmsstart, &tmsend);

    kill(getpid(), SIGTERM);
    kill(getpid(), SIGKILL);
    _exit(EXIT_SUCCESS);
}

void print_directory()
{
    char buf[MAXLINE];
    if (getcwd(buf, sizeof(buf)) != NULL)
    {
        printf("%s\n", buf);
    }
    else
    {
        fprintf(stderr, "%s", "Could not print current directory\n");
    }
}

void run_command(char *command)
{
    // clock_t startTime, endTime;
    // struct tms tmsstart, tmsend;
    char token[MAX_NTOKEN][MAXLINE];
    memmove(command, command + 4, strlen(command));
    int argtoken_size = split(command, token, " ");

    pid_t pid = fork();
    // printf("%d",pid);
    if (pid < 0)
    {
        fprintf(stderr, "%s", "Fork was not successfull");
    }
    else if (pid == 0)
    {
        // printf( "This is being printed from the child process\n" );
        // Child

        switch (argtoken_size)
        {
        case 1:
            if (execlp(token[0], token[0], (char *)NULL) < 0)
                _exit(EXIT_FAILURE);
            break;
        case 2:
            if (execlp(token[0], token[0], token[1], (char *)NULL) < 0)
                _exit(EXIT_FAILURE);
            break;
        case 3:
            if (execlp(token[0], token[0], token[1], token[2], (char *)NULL) < 0)
                _exit(EXIT_FAILURE);
            break;
        case 4:
            if (execlp(token[0], token[0], token[1], token[2], token[3], (char *)NULL) < 0)
                _exit(EXIT_FAILURE);
            break;
        case 5:
            if (execlp(token[0], token[0], token[1], token[2], token[3], token[4], (char *)NULL) < 0)
                _exit(EXIT_FAILURE);
            break;
        default:
            printf("Error");
        }
    }
    else
    {
        // printf( "This is being printed in the parent process:\n"
        //       " - the process identifier (pid) of the child is %d\n", pid );
        // Parent
        if (waitpid(pid, NULL, WNOHANG) < 0)
        {
            printf("Wait falied");
        }
        // printf("Strting to track tasks No %d:", counter);
        if (counter < 32)
        {

            proc[counter].index = counter;
            proc[counter].pid = pid;
            strcpy(proc[counter].command, command);
            // printf("%d", proc[counter].pid);
            counter++;
            // if(counter > 1){
            // printf("%s", proc[counter-2].command);}
        }

        // endTime = times(&tmsend);

        // clock_t totalTime = endTime - startTime;
        // print_time(totalTime, &tmsstart, &tmsend);
    }
    // All tasks must be recorded
}
