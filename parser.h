/*
 * parser.h: contains functions used to parse text
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char **command_parser(char *command, char **arguments);

char *config_file_parser(char *variable);


