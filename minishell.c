#include "minishell.h"

void	pipex(mini_t *mini, char *path, char *input)
{
	char	**str;

	str = ft_split(input, ' ');
	path[ft_strlen(path) - 1] = '\0';
	dup2(mini->ffd[1], 1);
	close(mini->ffd[0]);
    close(mini->ffd[1]);
	execve(path, str, mini->env);
	perror("execve failed");
    exit(EXIT_FAILURE);
}

void	get_command(mini_t *mini, char *command)
{
	char	**str;

	str = ft_split(ft_strjoin("which ", command), ' ');
	dup2(mini->fd[1], 1);
	close(mini->fd[0]);
    close(mini->fd[1]);
	execve("/usr/bin/which", &str[0], mini->env);
}

int main(int ac, char **av, char **env)
{
    char *input;
	mini_t mini;
	char *str;

	mini.ac = ac;
	mini.av = av;
	mini.env = env;
	while (1)
	{
		input = readline("minishell$ ");
        if (!input)
		{
			printf("exit\n");
			break;
		}
        if (*input)
			add_history(input);
		pipe(mini.fd);
		mini.pid = fork();
		if (mini.pid == 0)
		{
			get_command(&mini, input);
		}
		close(mini.fd[1]);
		wait(NULL);
		mini.path = get_next_line(mini.fd[0]);
		if (!mini.path)
			printf("minishell: %s no such file or directory\n", input);
		close(mini.fd[0]);
		pipe(mini.ffd);
		mini.pid = fork();
		if (mini.pid == 0)
		{
			pipex(&mini, mini.path, input);
		}
		close(mini.ffd[1]);
		wait(NULL);
		while(str = get_next_line(mini.ffd[0]))
		{
			printf("%s", str);
			free(str);
		}
		free(input);
		close(mini.ffd[0]);
    }
    return 0;
}
