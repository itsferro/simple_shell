#include "main.h"

/*
 * main - a simple shell program.
 *
 * ac: the arguments count.
 * av: the arguments list.
 *
 * Return: void.
 */
int main(int ac, char **av, char **env)
{
	char *command = NULL, cwd[BUFFER], *username = NULL, hostname[BUFFER], *exec_path;
	size_t n = 0, i = 1;
	ssize_t nbytes_read = 0;

	env = environ;
	while (i == 1)
	{
		/**
		 * printf("#Simple_shell$ ");
		 * this code display the hostname,username, and current working directory
		 *
		 * char cwd[1024];
		 */
		i  = isatty(STDIN_FILENO);
		if (i == 1)
		{
			getcwd(cwd, sizeof(cwd));
		/**
		 * char *username = getlogin();
		 * char hostname[1024];
		 */
			gethostname(hostname, sizeof(hostname));
			username = getlogin();
			printf("%s@%s:%s$\t", username, hostname, cwd);
		}
		else
		{
			i = 0;
		}

		nbytes_read = getline(&command, &n, stdin);
		if (nbytes_read == -1)
		{
			perror("getline");
			free(command);
			exit(EXIT_FAILURE);
		}

		tokenize(command, &ac, av);
		if (strcmp(av[0], "exit") == 0)
		{
			free(command);
			exit(EXIT_SUCCESS);
		}
		if (strcmp(av[0], "env") == 0)
		{
			while (*env != NULL)
			{

				printf("%s\n", *env);
				env++;
			}
		}
		if (strcmp(av[0], "cd") == 0)
		{
			chdir(av[1]);
		}
		else
		{
			exec_path = get_the_executable_path(av[0]);
			if (strcmp(av[0], "") == 0)

			{
				printf("%s: command not found\n", command);
			}

			else
			{
				run_command(exec_path, av);
			}
		}
	}
	free(command);
	return (EXIT_SUCCESS);
}
