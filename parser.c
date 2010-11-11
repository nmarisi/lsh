#include <stdio.h>
#include "parser.h"

/*
This file contains 3 functions that parse text used in the shell.

The first one command_parser, is used to create the arguments array.
The second function config_file_parser, is used to read key value pairs of the type VARIABLE=value in the config file
The third function, dir_parser is used to parse the directory string that will follow cd 
*/


char **command_parser(char *command, char **arguments) 
//function to parse the string typed in the command line
// command is a pointer to the whle string, and arguments is a pointer to an array of pointers that will contain argv (command+arguments)

{

    char **arg_ptr = arguments;

    arguments[0]= command; //first element of arguments array has tobe the actual command
    arg_ptr++; //increment pointer in order to use arg[1] for first parameter

    while (*command !='\0') { //so long as we havent reached the end of the string
        if (*command==' ') { //if we find a space
            *command='\0'; //we replace it with a null character
            *arg_ptr=command+1; //we store the poistion after the space in the arguments array of pointers
            arg_ptr++; //inc the pointer in order to store next element
        }
        command++; //move to the next character of the string
    }
    *arg_ptr=NULL; //terminate teh arguments array with null pointer (as required by exec)
    return arguments;
}

char *config_file_parser(char *var)
// finds a variable in the config file and returns the value as a string array

{
    FILE *config_file;
    
    //need to do error checking and freeing memory
    char *prompt= malloc(sizeof(char)*80);

    if ((config_file=fopen("./shell-config","r")) == NULL) {
        printf("could not open shell-config file\n");
        } 
        else  {
            
            //max size of the config-file, everything else after 4096 bytes will be ignored
            //need to add some text to the buffer in order for sscanf to look for prompt in a string before
            //fgets is executed
            char buffer[4096]="EMPTY";
          
          
          //char *variable = malloc(sizeof(char)*80);
          char variable[80]="";
          if( (strncat(variable,var,sizeof(char)*80)) ==NULL) {
              printf("error reading path from config-file");
              }
            //  else if ( (strncat(variable,"=%s", sizeof(char)*80)) ==NULL)

              else if ( (strncat(variable,"=%s", (sizeof(variable) - strlen(variable) -1))) ==NULL)
                  printf("error appending s to path string");


            while ((sscanf(buffer,variable,prompt)) ==0) {
                
                if ( (fgets(buffer, 200, config_file)) ==NULL)
                    printf("could not read data from shell-config file problem here");
                else
                    sscanf(buffer,variable,prompt);
            }

       }
    if (fclose(config_file) !=0)
        perror("Error");
    return prompt; 
}

