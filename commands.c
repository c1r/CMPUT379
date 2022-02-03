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

#define MAXLINE     260
#define MAX_NTOKEN  MAXLINE
#define MAXWORD     32

/* struct processes{
    pid_t pid;
    char command[60];
    int index;

}proc[32]; */

int cd(char* pathname);
void print_directory();
void run_command(char* command);
void lstasks();
void check_task(pid_t);
void stop_task(int);
void continue_task(int);
void terminate_task(int);
void terminate_alltasks(clock_t, struct tms *tmsstart);
void quit_maintask(clock_t, struct tms *tmsstart);
void print_times(clock_t realTime, struct tms *tmsstart, struct tms *tmsend );
/* char* split(char inStr[],  char token[][MAXWORD], char fs[])
{
    int i, count;
    char *tokenp, inStrCopy[MAXLINE];
    count = 0;
    memset (inStrCopy, 0, sizeof(inStrCopy));
    for (i=0; i < MAX_NTOKEN; i++) memset (token[i], 0, sizeof(token[i]));

    strcpy (inStrCopy, inStr);
    if ( (tokenp= strtok(inStr, fs)) == NULL) return(0);

    strcpy(token[count],tokenp); count++;

    while ( (tokenp= strtok(NULL, fs)) != NULL) {
        strcpy(token[count],tokenp); count++;
    }
    strcpy (inStr, inStrCopy);
    return token;  
} */

/* void split(char *buf,char *array){
    int i=0;
    char *p = strtok(buf, " ");

    while (p!=NULL){
        strcpy(array[i++],p);
        p = strtok(NULL, " ");
    }
} */

int split(char inStr[],  char token[MAX_NTOKEN][MAXLINE], char fs[])
{
    int    i, count;
    char   *tokenp, inStrCopy[MAXLINE];

    count= 0;
    memset (inStrCopy, 0, sizeof(inStrCopy));
    for (i=0; i < MAXLINE; i++) memset (token[i], 0, sizeof(token[i]));

    strcpy (inStrCopy, inStr);
    if ( (tokenp= strtok(inStr, fs)) == NULL) return(0);

    strcpy(token[count],tokenp); count++;

    while ( (tokenp= strtok(NULL, fs)) != NULL) {
        strcpy(token[count],tokenp); count++;
    }
    strcpy (inStr, inStrCopy);
    return(count);
}

void input_p(char* input, clock_t startTime, struct tms *tmsstart){
char array[MAX_NTOKEN][MAXLINE];
//printf("I'm here first lads %s\n",input);
    split(input,array," ");
    //printf("First token %s",array[1]);
    if(strcmp(array[0], "cdir")==0){
        char pathname[sizeof(array[1])];
        strcpy(pathname,array[1]);
        cd(pathname);
    }
    else if(strcmp(array[0], "pdir")==0){
        print_directory();
    }
    else if(strcmp(array[0], "lstasks")==0){
        lstasks();
    }
    else if(strcmp(array[0], "check")==0){
        check_task(atoi(array[1])); 
    }
    else if(strcmp(array[0], "stop")==0){
        stop_task(atoi(array[1]));
    }
    else if(strcmp(array[0], "continue")==0){
        continue_task(atoi(array[1]));
    }
    else if(strcmp(array[0],"terminate")==0){
        terminate_task(atoi(array[1]));
    }
    else if(strcmp(array[0],"exit")==0){
        terminate_alltasks(startTime ,tmsstart);
    }
    else if(strcmp(array[0],"quit")==0){
        quit_maintask(startTime ,tmsstart);
    }
    else{
        run_command(input);
    }
}


int cd(char* pathname) {
    char final_directory[MAXLINE];
    //printf("%s Im here lads", pathname);

    if(pathname[0] == '$'){
        memmove(pathname, pathname+1, strlen(pathname));

    //To obtain env variable expansion and thus its directory
    char *env_varexp = strtok(pathname, "/\n");
    char *env_var_dir = getenv(env_varexp);
    if (env_var_dir == NULL) {
      printf("cd: no such environment variable: %s\n", env_varexp);
      return -1;
    }
   
    //Copying to final
    strcpy(final_directory,env_var_dir);

    //To obtain the directory after the env_var

    char *delim = "";
    char *rest = strtok(NULL, delim);

    if(rest != '\0'){
        //adding a '/' before appending the rest of the pathname directory
       strcat(final_directory,"/");
       strcat(final_directory,rest);
        
    }
}

    else{
        //Just the env_var pathname is copied to final
        strcpy(final_directory,pathname);
        }
    
    if (chdir(final_directory)!= 0){
        printf("cdir: failed (pathname= %s):\n", final_directory);
        return -1;
    }
    printf("cdir: done (pathname= %s)\n",final_directory);
    return 1;
}

void lstasks(){
   // printf("counter: %d", counter);
    for (int i=0; i<counter; i++){
        if(strcmp(proc[i].command,"")!=0){
        printf("%d: (pid: %d, cmd: %s)\n",proc[i].index,proc[i].pid,proc[i].command);
        }
    }
}

void check_task(pid_t pid){
    FILE *fp;
    int status=0;
    char path[MAXLINE];
    
    fp = popen("ps -u $USER -o user,pid,ppid,state,start,cmd --sort start", "r");
    if(fp){
        char tokens[MAX_NTOKEN][MAXLINE];
        while(fgets(path,MAXLINE,fp)!= NULL ){
            split(path, tokens," ");
            printf( "%s  %s  %s  %s  %s  %s  \n", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
        }
    }
    status = pclose(fp);
    if(status ==-1) printf("Close failed\n");
}



void stop_task(int task){
    if(proc[task].index == task){
        kill(proc[task].pid, SIGSTOP);
    }
    else{
        printf("Couldn't find task with index: %d", task);
    }

}

void continue_task(int task){
    if(proc[task].index == task){
        kill(proc[task].pid, SIGCONT);
    }
    else{
        printf("Couldn't find task with index: %d", task);
    }

}

void terminate_task(int task){
    if(proc[task].index == task){
        kill(proc[task].pid, SIGCONT);
        kill(proc[task].pid, SIGTERM);
        kill(proc[task].pid, SIGKILL);

        printf("%d: (pid: %d, cmd: %s) terminated\n",proc[task].index,proc[task].pid,proc[task].command);
        proc[task].index='\0';
        proc[task].pid='\0';
        strcpy(proc[task].command,"\0");
    
    }
    /* else{
        printf("Couldn't find task with index: %d\n", task);
    } */
}

void terminate_alltasks(clock_t startTime, struct tms *tmsstart){
    for(int i=0; i<counter; i++){
        terminate_task(i);
    }

    struct tms tmsend;
    clock_t endTime = times(&tmsend);
    if(endTime == -1){
        printf("Failed to record the end time\n");
    }
    print_times(endTime-startTime,tmsstart,&tmsend);
}


void quit_maintask(clock_t startTime, struct tms *tmsstart){
    

    struct tms tmsend;
    clock_t endTime = times(&tmsend);
    if(endTime == -1){
        printf("Failed to record the end time\n");
    }
    print_times(endTime-startTime,tmsstart,&tmsend);
    
    kill(getpid(), SIGTERM);
    kill(getpid(), SIGKILL);
    _exit(EXIT_SUCCESS);
}

void print_times(clock_t realTime, struct tms *tmsstart, struct tms *tmsend){
    long clktck = 0;

    if ( clktck == 0 ) {
        if ( (clktck = sysconf(_SC_CLK_TCK)) < 0 ) {
            printf( "Failed to fetch clock ticks per second\n" );
        }
    }

    printf( "   real:%12.2f\n", realTime / (double) clktck );
    
    printf( "   user:%12.2f\n", 
        ( tmsend->tms_utime - tmsstart->tms_utime ) / (double) clktck );
    
    printf( "   sys:%13.2f\n", 
        ( tmsend->tms_stime - tmsstart->tms_stime ) / (double) clktck );
    
    printf( "   child user:%6.2f\n", 
        ( tmsend->tms_cutime - tmsstart->tms_cutime ) / (double) clktck );

    printf( "   child sys:%7.2f\n", 
        ( tmsend->tms_cstime - tmsstart->tms_cstime ) / (double) clktck );
}

void print_directory(){
    char buf[MAXLINE];
    if(getcwd(buf, sizeof(buf)) != NULL){
        printf("%s\n", buf);
    }
    else {
        fprintf(stderr, "%s", "Could not print current directory\n");
    }
}

void run_command(char* command){
    //clock_t startTime, endTime;
    //struct tms tmsstart, tmsend;
    char token[MAX_NTOKEN][MAXLINE];
    memmove(command, command+4, strlen(command));
    int argtoken_size = split(command,token," ");
    //int taskNo = (sizeof(proc) /sizeof(proc[0]));
    //printf("%d ", argtoken_size);
    
    

    //tokens[sizeof(tokens)-1]='\0';
    //printf("I'm here bitches\n");

    //startTime = times(&tmsstart);

    pid_t pid = fork();
    //printf("%d",pid);
    if (pid < 0){
        fprintf(stderr, "%s", "Fork was not successfull");
    }
    else if(pid ==0){
        //printf( "This is being printed from the child process\n" );
        //Child

        switch(argtoken_size){
            case 1: if(execlp(token[0],token[0],(char*)NULL)<0) _exit(EXIT_FAILURE);
            break;
            case 2: if(execlp(token[0],token[0],token[1],(char*)NULL)<0) _exit(EXIT_FAILURE);
            break;
            case 3: if(execlp(token[0],token[0],token[1],token[2],(char*)NULL)<0) _exit(EXIT_FAILURE);
            break;
            case 4: if(execlp(token[0],token[0],token[1],token[2],token[3],(char*)NULL)<0) _exit(EXIT_FAILURE);
            break;
            case 5: if(execlp(token[0],token[0],token[1],token[2],token[3],token[4],(char*)NULL)<0) _exit(EXIT_FAILURE);
            break;
            default: printf("Error");
        }
    }
    else {
        //printf( "This is being printed in the parent process:\n"
		  //      " - the process identifier (pid) of the child is %d\n", pid );
        //Parent
        if(waitpid(pid, NULL, WNOHANG)<0){
            printf("Wait falied");
        }
        //printf("Strting to track tasks No %d:", counter);
        if(counter<32){
            
            proc[counter].index= counter;
            proc[counter].pid= pid;
            strcpy(proc[counter].command,command);
            //printf("%d", proc[counter].pid);
            counter++;
            //if(counter > 1){
            //printf("%s", proc[counter-2].command);}

        }

       // endTime = times(&tmsend);

        //clock_t totalTime = endTime - startTime;
        //print_time(totalTime, &tmsstart, &tmsend);
            


    }
    //All tasks must be recorded

}


