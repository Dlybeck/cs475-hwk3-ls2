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

//Mode 1. No Exact match
void runls (char path[], int indents){
    DIR* dir;
    dir = opendir(path);
    if(dir == NULL){
        printf("This path does not exist");
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
                printf("(%ld) bytes\n", size);
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

//mode 2
/*void runls2 (char* path, char* match, stack_t* stack, int indents){
    DIR* dir;
    dir = opendir(path);
    if(dir == NULL){
        printf("This path does not exist");
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
                    long size = fileInfo.st_size;

                    //Create the properly indented file name
                    char* indents;
                    for(int i = 0; i < indents; i++) strcat(indents, INDENT);
                    char* output = sprintf("(%ld) bytes\n", size);
                    output = strcat(indents, output);
                    push(stack, output);

                    //Print everything added so far
                    printstack(stack);

                    //Empty the stack
                    while(stack->size > 0){
                        pop(stack);
                    }
                }
            }
            //it is a directory
            else{
                //Create the properly indented file name
                char* spaces;
                for(int i = 0; i < indents; i++) strcat(spaces, INDENT);
                char* output;
                sprintf(output, "/ (directory)\n");
                output = strcat(indents, output);
                push(stack, output);
                indents++;

                push(stack, output);
                runls2(fullPath, match, stack, indents);
                pop(stack);
            }
        }
    }
}*/
