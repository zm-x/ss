#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include "get_next_line.h"

typedef struct mini
{
	int	fd[2];
	pid_t pid;
	char **av;
	int ac;
    char *path;
	char **env;
	int ffd[2];
}	mini_t;

char	**ft_split(char const *s, char c);

#endif