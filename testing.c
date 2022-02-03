#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "test.h"

#define MAXLINE     132
#define MAX_NTOKEN  MAXLINE
#define MAXWORD     32

char *split(char inStr[],  char token[][MAXWORD], char fs[])
{
    int    i, count;
    char   *tokenp, inStrCopy[MAXLINE];
    count= 0;
    memset (inStrCopy, 0, sizeof(inStrCopy));
    for (i=0; i < MAX_NTOKEN; i++) memset (token[i], 0, sizeof(token[i]));

    strcpy (inStrCopy, inStr);
    if ( (tokenp= strtok(inStr, fs)) == NULL) return(0);

    strcpy(token[count],tokenp); count++;

    while ( (tokenp= strtok(NULL, fs)) != NULL) {
        strcpy(token[count],tokenp); count++;
    }
    strcpy (inStr, inStrCopy);
    return *token;  
}


int main(){

    char command[MAXLINE];
    fgets(command, MAXLINE,stdin);
    command[sizeof(command)-1]='\0';
    char tok[512][MAXWORD];
    split(command,tok," ");
    char *newo = tok[2];
    
    printf("%s1 \n",tok);
    printf("%s2 \n", newo);
    //     if(newo[0] == 'l'){
    //     memmove(newo, newo+1, strlen(newo));}
    // printf("%3s", newo);
    //int sizeo = strlen(tok);
    //printf("%d", sizeo);
    return 0;
}