#ifndef msh379_h
#define msh379_h


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/stat.h>

struct processes{
    pid_t pid;
    char command[60];
    int index;
};
extern struct processes proc[32];
extern int counter;

//void get_time(char* buffer);
//void get_sysinfo();
//void print_error(int error);
//void input_p(char* input);

#endif
