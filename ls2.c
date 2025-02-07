#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

char* PathName(char* path, char* fileName){
    int len = strlen(path) + strlen(fileName) + 1 + 1; //One for the / the other for the \0 at the end of a string
    char* newName = (char*) malloc(len);
    memset(newName, 0, len);
    strcat(newName, path);
    strcat(newName, "/");
    strcat(newName, fileName);
    return newName;
}

void printreversestack(stack_t* s) {
    stack_t *reverse = initstack();
    stacknode_t *current = s->top;
    //"Flip" the stack
    while (current != NULL) {
        push(reverse, current->data);
        current = current->next;
    }
    stacknode_t *newNode = reverse->top;
    //Print out the reversed stack
    while (newNode != NULL) {
        printf("%s", (char*) newNode->data);
        newNode = newNode->next;
    }
    freestack(reverse);
}

//Mode 1. No Exact match
void runls (char path[], int indents){
    DIR* dir;
    dir = opendir(path);
    if(dir == NULL){
        printf("The path %s does not exist", path);
        return;
    }

    struct dirent *file;
    while((file = readdir(dir)) != NULL){
        char* name = file->d_name;
        //ignore . and .. to prevent infinate loops
        if((strcmp(name, ".") !=0 ) && (strcmp(name, "..") != 0)){
            for(int i = 0; i < indents; i++) printf(INDENT); //Add indents
            printf("%s", name); //print entry name to output
            char* fullPath = PathName(path, name); //Create full path

            //create fileInfo stat
            struct stat fileInfo;
            lstat(fullPath, &fileInfo);

            //if it is a file
            if(S_ISREG(fileInfo.st_mode)){
                long size = fileInfo.st_size;
                printf("(%ld bytes)\n", size);
            }
            //if it is a folder
            else{
                printf("/ (directory)\n");
                runls(fullPath, indents + 1);
            }
            free(fullPath);
        }
    }
    closedir(dir);
}

//mode 2 w/ exact match
void runls2 (char* path, char* match, stack_t* stack, int indents){
    DIR* dir;
    dir = opendir(path);
    if(dir == NULL){
        printf("The path %s does not exist", path);
        return;
    }

    struct dirent *file;
    while((file = readdir(dir)) != NULL){
        char* name = file->d_name;
        //ignore . and .. to prevent infinate loops
        if((strcmp(name, ".") !=0 ) && (strcmp(name, "..") != 0)){
            char* fullPath = PathName(path, name); //Create full path

            //create fileInfo stat
            struct stat fileInfo;
            lstat(fullPath, &fileInfo);
            //if it is a file
            if(S_ISREG(fileInfo.st_mode)){
                //if it matches what is wanted
                if(strcmp(name, match) == 0){
                    long int size = fileInfo.st_size;

                    // Allocate memory for formatted string
                    char sizeStr[255]; // Just set to max chars for filename?
                    // Format the size string
                    int num_chars = snprintf(sizeStr, sizeof(sizeStr), "(%ld bytes)\n", size);
                    int len = (strlen(INDENT) * indents) + strlen(name) + num_chars + 1;
                    char *fullEntry = (char *)malloc(len);
                    memset(fullEntry, 0, len);

                    for(int i = 0; i < indents; i++) strcat(fullEntry, INDENT); //Add the proper # of indents to the name
                    strcat(fullEntry, name);
                    strcat(fullEntry, sizeStr);
                    push(stack, fullEntry); //Add the file name to the stack

                    //Print everything added so far
                    printreversestack(stack);

                    //Empty the stack
                    while (stack->size > 0) {
                         pop(stack);  
                    }
                }
            }
            //it is a directory
            else{
                //Create the properly indented file name
                int len = (strlen(INDENT)*indents) + strlen(name) + strlen("/ (directory)\n") + 1;
                char* fullEntry = (char*) malloc(len);
                memset(fullEntry, 0, len);

                for(int i = 0; i < indents; i++) strcat(fullEntry, INDENT); //Add the proper # of indents to the name
                strcat(fullEntry, name);
                strcat(fullEntry, "/ (directory)\n");
                push(stack, fullEntry); //Add the directory to the stack

                indents++; //increment indents before recursion
                runls2(fullPath, match, stack, indents);
                indents--;  //decrement for when returning back to earlier folders
                free(pop(stack)); //In case the directory was empty, backtrack (Otherwise this does nothing)
            }
            free(fullPath);
        }
    }
    closedir(dir);
}