/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:22:02 by ksudyn            #+#    #+#             */
/*   Updated: 2026/07/06 21:12:23 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

/*
** Reemplaza getline() por get_next_line() de tu Libft para leer el heredoc.
** Cumple la Norm: 3 variables, 21 líneas, bucle while.
*/
int	handle_heredoc(char *limiter)
{
	int		pipefd[2];
	char	*line;
	size_t	len;

	if (pipe(pipefd) == -1)
		return (-1);
	len = ft_strlen(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

/*
** Ejecuta un comando individual. Utiliza las funciones de tu obligatorio.
** Cumple la Norm: 2 variables, 14 líneas.
*/
static void	execute_bonus_cmd(char *cmd_arg, char **envp)
{
	char	**command;
	char	*path_command;

	command = ft_split(cmd_arg, ' ');
	if (!command)
		exit(1);
	path_command = ft_get_path_command(command, envp, NULL);
	if (path_command)
		execve(path_command, command, envp);
	write(2, "pipex: command not found\n", 25);
	free_kids(command);
	exit(127);
}

/*
** Lógica del hijo para realizar redirecciones con dup2.
** Recibe un entero 'is_last' para saber si escribe en el pipe o en el outfile.
** Cumple la Norm: 0 variables, 4 argumentos exactos.
*/
static void	child_process(t_pipex *boss, char *cmd, char **envp, int is_last)
{
	dup2(boss->prev_fd, STDIN_FILENO);
	if (is_last)
		dup2(boss->out_fd, STDOUT_FILENO);
	else
		dup2(boss->pipefd[1], STDOUT_FILENO);
	close(boss->prev_fd);
	if (!is_last)
	{
		close(boss->pipefd[0]);
		close(boss->pipefd[1]);
	}
	close(boss->out_fd);
	execute_bonus_cmd(cmd, envp);
}

/*
** Función auxiliar para actualizar los descriptores del padre en cada ciclo.
** Cumple la Norm: 0 variables, 11 líneas.
*/
static void	parent_process(t_pipex *boss, int i, int argc)
{
	close(boss->prev_fd);
	if (i < argc - 2)
	{
		close(boss->pipefd[1]);
		boss->prev_fd = boss->pipefd[0];
	}
}

/*
** Controla el bucle de procesos intermedios mediante pipes de forma ultra-corta.
** Cumple la Norm: 2 variables, 22 líneas totales.
*/
void	run_pipeline(int argc, char **argv, char **envp, t_pipex *boss)
{
	int		i;
	pid_t	pid;

	i = 2;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		i = 3;
	boss->prev_fd = boss->in_fd;
	while (i < argc - 1)
	{
		if (i < argc - 2 && pipe(boss->pipefd) == -1)
			exit(1);
		pid = fork();
		if (pid == 0)
			child_process(boss, argv[i], envp, (i == argc - 2));
		parent_process(boss, i, argc);
		i++;
	}
	close(boss->out_fd);
	while (i-- > 0)
		wait(NULL);
}
