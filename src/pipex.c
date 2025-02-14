/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:25:35 by ksudyn            #+#    #+#             */
/*   Updated: 2025/02/11 12:25:37 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_kid_1(int *fd, int *int_array, char **kid_1, char **env)
{
	char *path_command;
	char *path_mid;

	// Cerrar extremo de lectura del pipe
	close(int_array[0]);

	// Redirigir la entrada estándar (leer de infile)
	dup2(fd[0], 0);
	close(fd[0]);
	close(fd[1]);

	// Redirigir la salida estándar (escribir en el pipe)
	dup2(int_array[1], 1);
	close(int_array[1]);

	// Obtener la ruta del comando
	path_mid = NULL;
	path_command = ft_get_path_command(kid_1, env, path_mid);

	// Ejecutar el comando
	if (path_command)
		execve(path_command, kid_1, env);

	free_kids(kid_1);
}

int	ft_kid_2(int *fd, int *int_array, char **kid_2, char **env)
{
	char *path_command;
	char *path_mid;

	// Cerrar extremo de escritura del pipe
	close(int_array[1]);

	// Redirigir la entrada estándar (leer del pipe)
	dup2(int_array[0], 0);
	close(int_array[0]);

	// Redirigir la salida estándar (escribir en outfile)
	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);

	// Obtener la ruta del comando
	path_mid = NULL;
	path_command = ft_get_path_command(kid_2, env, path_mid);

	// Ejecutar el comando
	if (path_command)
		execve(path_command, kid_2, env);

	free_kids(kid_2);
	exit(127);
}

int	ft_pipex(int *fd, char **kid_1, char **kid_2, char **env)
{
	int tube[2];
	pid_t children_1;
	pid_t children_2;
	int status;

	// Crear el pipe (int_array[0] → lectura, int_array[1] → escritura)
	pipe(tube);

	// Crear el primer hijo (ejecuta kid_1)
	children_1 = fork();
	if (children_1 == 0)
	{
		free_kids(kid_2);
		ft_kid_1(fd, tube, kid_1, env);
	}

	// Crear el segundo hijo (ejecuta kid_2)
	children_2 = fork();
	if (children_2 == 0)
	{
		free_kids(kid_1);
		status = ft_kid_2(fd, tube, kid_2, env);
	}

	// Cerrar extremos del pipe en el padre
	close(tube[0]);
	close(tube[1]);

	// Esperar a que los hijos terminen
	waitpid(children_1, &status, 0);
	waitpid(children_2, &status, 0);

	return (WEXITSTATUS(status));
}

