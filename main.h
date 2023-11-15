#ifndef MAIN
#define MAIN

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER 1024

extern char **environ;

void tokenize(char *command, int *ac, char **av);
char *get_the_executable_path(char *av);
int run_command(char *exec_path, char **av);

#endif /*the end of #ifndef MAIN*/
