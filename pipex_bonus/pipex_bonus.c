/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:22:02 by ksudyn            #+#    #+#             */
/*   Updated: 2025/06/20 17:30:11 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

extern char	**ft_split_quotes(char *input, char c);
extern char	*get_cmd_path(char *cmd, char **envp);

// Función para heredoc
int	handle_heredoc(char *limiter)
{
	int		pipefd[2];
	char	*line = NULL;
	size_t	len = 0;

	if (pipe(pipefd) == -1)
		perror("pipe");
	while (1)
	{
		write(1, "heredoc> ", 9);
		getline(&line, &len, stdin);
		if (!line || strncmp(line, limiter, strlen(limiter)) == 0)
			break;
		write(pipefd[1], line, strlen(line));
	}
	free(line);
	close(pipefd[1]);
	return pipefd[0];
}

void	exec_cmd(char *cmd_str, char **envp)
{
	char	**args = ft_split_quotes(cmd_str, ' ');
	char	*cmd_path = get_cmd_path(args[0], envp);
	if (!cmd_path)
	{
		perror("command not found");
		exit(127);
	}
	execve(cmd_path, args, envp);
	perror("execve");
	exit(1);
}

void	run_pipeline(int argc, char **argv, char **envp, int heredoc)
{
	int		i, in_fd, out_fd, prev_fd = -1;
	int		pipefd[2];
	pid_t	pid;

	int	cmd_start = heredoc ? 3 : 2;
	int	cmd_end = argc - 2;

	if (heredoc)
		in_fd = handle_heredoc(argv[2]);
	else
		in_fd = open(argv[1], O_RDONLY);

	out_fd = open(argv[argc - 1],
			O_CREAT | O_WRONLY | (heredoc ? O_APPEND : O_TRUNC), 0644);

	for (i = cmd_start; i <= cmd_end; i++)
	{
		if (i < cmd_end && pipe(pipefd) == -1)
			perror("pipe");

		pid = fork();
		if (pid == 0)
		{
			if (i == cmd_start)
				dup2(in_fd, STDIN_FILENO);
			else
				dup2(prev_fd, STDIN_FILENO);

			if (i == cmd_end)
				dup2(out_fd, STDOUT_FILENO);
			else
				dup2(pipefd[1], STDOUT_FILENO);

			if (in_fd != STDIN_FILENO) close(in_fd);
			if (out_fd != STDOUT_FILENO) close(out_fd);
			if (prev_fd != -1) close(prev_fd);
			if (i < cmd_end)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			exec_cmd(argv[i], envp);
		}
		else if (pid < 0)
			perror("fork");

		if (prev_fd != -1) close(prev_fd);
		if (i < cmd_end)
		{
			prev_fd = pipefd[0];
			close(pipefd[1]);
		}
	}
	close(in_fd);
	close(out_fd);
	while (wait(NULL) > 0);
}


#include "pipex_bonus.h"

int main(int argc, char **argv, char **envp)
{
    if (argc < 5)
        return (ft_error_exit("Invalid number of arguments"), 1);

    if (ft_strcmp(argv[1], "here_doc") == 0)
        return (execute_here_doc(argc, argv, envp));
    return (execute_pipeline(argc, argv, envp));
}


