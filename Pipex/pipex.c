/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:25:35 by ksudyn            #+#    #+#             */
/*   Updated: 2026/07/06 21:05:01 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_kid_1(int *fd, int *tube, char **command_1, char **env)
{
	char	*path_command;
	char	*path_temp;

	close(tube[0]);
	dup2(fd[0], 0);
	close(fd[0]);
	close(fd[1]);
	dup2(tube[1], 1);
	close(tube[1]);
	path_temp = NULL;
	path_command = ft_get_path_command(command_1, env, path_temp);
	if (path_command)
		execve(path_command, command_1, env);
	perror("error command");
	free_kids(command_1);
	exit(127);
}

int	ft_kid_2(int *fd, int *tube, char **command_2, char **env)
{
	char	*path_command;
	char	*path_mid;

	close(tube[1]);
	dup2(tube[0], 0);
	close(tube[0]);
	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
	path_mid = NULL;
	path_command = ft_get_path_command(command_2, env, path_mid);
	if (path_command)
		execve(path_command, command_2, env);
	perror("error command");
	free_kids(command_2);
	exit(127);
}

int	ft_pipex(int *fd, char **command_1, char **command_2, char **env)
{
	int		tube[2];
	pid_t	children_1;
	pid_t	children_2;
	int		status;

	pipe(tube);
	children_1 = fork();
	if (children_1 == 0)
	{
		free_kids(command_2);
		ft_kid_1(fd, tube, command_1, env);
	}
	children_2 = fork();
	if (children_2 == 0)
	{
		free_kids(command_1);
		status = ft_kid_2(fd, tube, command_2, env);
	}
	close(tube[0]);
	close(tube[1]);
	waitpid(children_1, &status, 0);
	waitpid(children_2, &status, 0);
	return (0);
}
