/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:20:59 by zael-mou          #+#    #+#             */
/*   Updated: 2025/02/18 17:49:18 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int ll = 0;

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

int	count_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	mini_pipex(shell_t *shell, char *path, char *input)
{
	char	**str;

	str = ft_split(input, ' ');
	path[ft_strlen(path) - 1] = '\0';
	execve(path, str, shell->env);
	perror("execve failed");
    exit(1);
}

void	get_command(shell_t *shell, char *command)
{
	char	**str;

	str = ft_split(ft_strjoin("which ", command), ' ');
	dup2(shell->fd[1], 1);
	close(shell->fd[0]);
    close(shell->fd[1]);
	execve("/usr/bin/which", &str[0], shell->env);
	perror("execve failed");
    exit(1);
}

void	put_command(shell_t *shell, char *input)
{
		pipe(shell->fd);
		shell->pid = fork();
		if (shell->pid == 0)
			get_command(shell, input);
		close(shell->fd[1]);
		wait(NULL);
		shell->path = get_next_line(shell->fd[0]);
		if (!shell->path)
			printf("minishell: %s no such file or directory\n", input);
		close(shell->fd[0]);
}

int	find_thing(char *av, char c)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (av[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	child(shell_t *shell, int i)
{
	dup2(shell->fd[1], 1);
	close(shell->fd[0]);
    close(shell->fd[1]);
	put_command(shell, shell->d_input[i]);
	mini_pipex(shell, shell->path, shell->d_input[i]);
}

void	middle_child(shell_t *shell, int i)
{
	dup2(shell->fd[1], 1);
	dup2(shell->prev_fd, 0);
	close(shell->fd[0]);
    close(shell->fd[1]);
	close(shell->prev_fd);
	put_command(shell, shell->d_input[i]);
	mini_pipex(shell, shell->path, shell->d_input[i]);
}

void	last_child(shell_t *shell, int i)
{
	int	fd;

	if (shell->redir == 1)
	{
		fd = open(shell->d_input[shell->input_len - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, 1);
		close(fd);
	}
	dup2(shell->prev_fd, 0);
	close(shell->prev_fd);
	put_command(shell, shell->d_input[i]);
	mini_pipex(shell, shell->path, shell->d_input[i]);
}

void	real_pipex(shell_t *shell)
{
	int	i;

	i = 1;
	shell->d_input = ft_split(shell->input, '|');
	shell->input_len = count_array(shell->d_input);
	pipe(shell->fd);
	shell->pid = fork();
	if (shell->pid == 0)
		child(shell, 0);
	close(shell->fd[1]);
	shell->prev_fd = shell->fd[0];
	while (i < shell->input_len - 1)
	{
		pipe(shell->fd);
		shell->pid = fork();
		if (shell->pid == 0)
			middle_child(shell, i);
		close(shell->prev_fd);
		close(shell->fd[1]);
		shell->prev_fd = shell->fd[0];
		i++;
	}
	shell->pid = fork();
	if (shell->pid == 0)
		last_child(shell, i);
	close(shell->prev_fd);
	while (wait(NULL) > 0)
		;
}

int	command_len(shell_t *shell , char *input)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	shell->index = ll;
	while (input[ll] == ' ' && input[ll])
		ll++;
	while (input[ll] && input[ll] != ' ')
	{
		j++;
		ll++;
	}
	while (input[ll] == ' ' && input[ll])
	{
		ll++;
		k++;
	}
	if (input[ll] == '-')
	{
		j += k;
		while (input[ll] && input[ll] != ' ')
		{
			j++;
			ll++;
		}
	}
	return (j);
}

char	*alloc_command(int i, char *input, int start)
{
	char	*str;
	int	l;

	//if (!i)
		//return (0);
	l = 0;
	str = malloc(i + 1);
	str[i] = '\0';
	while (l < i)
		str[l++] = input[start++];
	return(str);
}

void	handle_input(shell_t *shell)
{
	shell->command = alloc_command(command_len(shell, shell->input), shell->input, shell->index);
	printf("command = |%s|\n", shell->command);
	shell->command = alloc_command(command_len(shell, shell->input), shell->input, shell->index);
	printf("command = |%s|\n", shell->command);
}

int main(int ac, char **av, char **env)
{
	shell_t shell;
	char *str;

	shell.env = env;
	shell.redir = 0;
	while  (1)
	{
		shell.input = readline("minishell$ ");
		if (!shell.input)
			return(printf("exit\n"), 0);
		if (shell.input)
			add_history(shell.input);
		handle_input(&shell);
		ll = 0;
		// if (find_thing(shell.input, '>'))
		// 	shell.redir = 1;
		// if (find_thing(shell.input, '|'))
		// 	real_pipex(&shell);
		// else
		// {
		// 	put_command(&shell, shell.input);
		// 	shell.pid = fork();
		// 	if (shell.pid == 0)
		// 		mini_pipex(&shell, shell.path, shell.input);
		// 	wait(NULL);
		// }
		free(shell.input);
    }
    return 0;
}
