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
        }
    }
}
