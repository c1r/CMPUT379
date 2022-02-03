#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAXLINE     132
#define MAX_NTOKEN  MAXLINE
#define MAXWORD     32

char* split(char inStr[],  char token[][MAXWORD], char fs[]);

