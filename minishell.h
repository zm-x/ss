/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:12:47 by zael-mou          #+#    #+#             */
/*   Updated: 2025/02/18 17:45:37 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include "get_next_line.h"
#include <fcntl.h>
#include <signal.h>

typedef struct shell
{
	int	fd[2];
	pid_t pid;
    char *path;
	char **env;
	char *input;
	int	prev_fd;
	char	**d_input;
	int	input_len;
	int	redir;
	char	*command;
	char	*to_do;
	int	index;
}	shell_t;

char	**ft_split(char const *s, char c);

#endif