#include "main.h"

/*
 * my_strtok - making tokens from a string.
 *
 * str: a string.
 * c: a string of characters to make tokens at.
 *
 * Return: a string.
 */
char *my_strtok(char *str, char *c)
{
	static char *buffer = NULL;
	char *token;

	if (!buffer)
	{
		buffer = str;
	}
	if (!*buffer)
	{
		return (NULL);
	}

	token = buffer;
	while (*buffer && *buffer != *c)
	{
		buffer += 1;
	}

	if (*buffer)
	{
		buffer = NULL;
		buffer += 1;
	}

	return (token);
}

/*
 * tokenize - getting the arguments from the command.
 *
 * command: the command line.
 * ac: pointer to the arguments count.
 * av: pointer to the arguments list.
 *
 * Return: an array of arrays.
 */
void tokenize(char *command, int *ac, char **av)
{
	char *token = NULL;
	int i = 0;

	*ac = 0;
	token = strtok(command, " \n");
	while (token != NULL)
	{
		av[i] = token;
		token = strtok(NULL, " \n");
		i++;
	}
	av[i] = NULL;
	*ac = i;
}

/**
 * *get_the_executable_path - getting the execution path.
 *
 * av: is a string.
 *
 * Return: the path string on success and NULL if path no found.
 */
char *get_the_executable_path(char *av)
{
	int status, pipefd[2];
	pid_t child_pid;
	char buffer[4096], *argv[] = {"/bin/bash", "which", "av", NULL};
	ssize_t nbytes_read;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	child_pid = fork();
	if (child_pid == -1)
	{

		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
		close(pipefd[READ_END]);
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[WRITE_END]);
		argv[2] = av;
		execve(argv[0], argv, environ);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipefd[WRITE_END]);
		nbytes_read = read(pipefd[READ_END], buffer, sizeof(buffer));
		if (nbytes_read == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		while (nbytes_read < 4096)
		{
			buffer[nbytes_read] = '\0';
			nbytes_read++;
		}
		close(pipefd[READ_END]);
		wait(&status);
		av = buffer;
		strtok(av, " \n");
		return (av);
	}
}

/*
 * run_command - execute the command.
 *
 * av: the arguments list.
 *
 * Return: EXIT_SUCCESS on success and EXIT_FAILURE on failure.
 */
int run_command(char *exec_path, char **av)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
		execve(exec_path, av, environ);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&status);
	}
	return (EXIT_SUCCESS);
}
