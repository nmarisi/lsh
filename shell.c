#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "parser.h"
#include <dirent.h>

#define MAX_ARGS 20 /*maximum number of arguments per command */
#define MAX_ENV 1024 /* maximum length of any environment variable */

void change_dir (char *new_dir)
{
    if (chdir(new_dir)==-1) {
        printf("changing directory to %s\n",new_dir);
        //perror("Error");
    }
}

char *attach_path(char *command, char *path_var)
{
    //path_var: stores the content of the PATH variable in the shell config file
    //search_path, an array of pointers, where each element points to a path
    //path array will a pointer to every string tha represents a path 
    
    char *path_array[MAX_ENV/3]; //worst case scenario, all paths are one character /b /a /c /d + the : it's 3 char, div 3
  
    char **search_path=path_array;
   
    int pcounter=1; // number of paths (there will be one less : than the number of paths)

    if ( (*path_var==' ') || (*path_var=='\0') || (*path_var=='\n')) { 
        //if the path string is empty
        pcounter=0; //number of paths
    }
    
    
    //store the first path 
    search_path[0]=path_var;
    //move pointer to the next element
    search_path++;
    
    while (*path_var!='\0') {
        if (*path_var==':') {
            *path_var='\0';
            *search_path=path_var+1; //save the position of the next string
            search_path++;
            pcounter++;
        }
        path_var++;
    }
     *search_path=NULL; //terminate the search_path array with null

    //printf("path 1 is %s, number of paths is %i\n",path_array[1], pcounter);
    
    //need to do error checking

    struct dirent *dp=NULL; //structure that holds a directory entry
    DIR *dirp=NULL;  //a pointer to a directory stream
    char *path_to_command; /* variable to store the path where the command is */
    char is_found=0; // set to true when the first instance of the command is found
    int i;
    for (i=0; i<pcounter && !is_found; i++) { 
        //printf("inside the for loop\n");
        dirp= opendir(path_array[i]); //open directory stream
        while ((dp = readdir(dirp)) != NULL) { //readdir returns the directory structure and moves a pointer forward to then ext
            if (!strcmp(dp->d_name, command)) { //compare the name
                //printf("found command %s in %s\n", command, path_array[i]);
                path_to_command=path_array[i];
                is_found=1;
                char *final_command=malloc(sizeof(100));
                final_command=strncpy(path_to_command,command,100);
                return final_command;    

            }
        }
     (void)closedir(dirp);
    }
    return "not found";
    //need to do error checking
}


int run_command (char **arguments, char *path)
// takes a pointer to a character array which contains the command
// takes a pointer to the path
// second parameter is an array of character points, each pointing to a arguments (argv)
// ftrlen(name);
{

    pid_t pid;

    //fork the child process
    pid = fork();
    if (pid <0) {
        fprintf(stderr, "Fork Failed!");
        return 1;
    }

    else if (pid ==0)  { //child process as fork () returns 0 to the child
    attach_path("testcmd",path);
    
        if(execvp(attach_path(arguments[0],path),arguments)==-1) //returns -1 if there is an error and stores the code in the variable errno
            perror("Error"); //prints a text message for the variable errno
    }
    else { //parent process
        wait(NULL); //wait for child to complete
        //printf("Child complete\n");
    }
return 0;
}

            








int main(int argc, char **argv)
{
    
    printf("Welcome to LSH\n"); 
    printf("Have a nice christmas!\n\n");


    char buffer[255];
    char *prompt=config_file_parser("PROMPT");
    char *path = config_file_parser("PATH");
    //printf("the path is %s \n",path);
   
    
    while(1) {
       printf("%s ",prompt);
        if ( (fgets(buffer, sizeof(buffer), stdin)) == NULL)
            printf("error reading input");
            else {
                //need to remove the newline character that fgets stores and add a \0 to end of string
                char *ptr =strchr(buffer, '\n');
                if (ptr)
                    *ptr='\0';
            }

        if (strcmp(buffer, "exit")==0)
        {
           exit(0);
        }
        //compare the first two characters of the user input string to see if they contain "cd"
        //if it matches, find the space between cd and the directory name, and save a pointer to the directory name
        if (strncmp(buffer, "cd", sizeof(char)*2)==0) {
            char *ptr=buffer;
            while (*ptr!=' ')
                ptr++;
            ptr++;

           change_dir(ptr);
        } else {
            char *arguments[MAX_ARGS];
            run_command(command_parser(buffer,arguments),path);
        }
    }
    
    return 0;
}
