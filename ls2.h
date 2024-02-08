#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "

// TODO: Function declarations here for ls2
void runls(char path[], int indents);

void runls2 (char* path, char* match, stack_t* stack, int indents);

void printreversestack(stack_t* s);

char* PathName(char* path, char* fileName);



#endif
