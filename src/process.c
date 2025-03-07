/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:26:01 by ksudyn            #+#    #+#             */
/*   Updated: 2025/02/11 12:26:03 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_check_command(char **path_list, char *path_temp, char *command)
{
	int		i;
	char	*path_result;

	i = -1;
	while (path_list[++i])
	{
		path_temp = ft_strjoin(path_list[i], "/");
		path_result = ft_strjoin(path_temp, command);
		free(path_temp);
		if (access(path_result, X_OK) == 0)
		{
			free_path_list(path_list);
			return (path_result);
		}
		free(path_result);
	}
	return (NULL);
}

//esta funcion busca en la lista de comandos separados por / 
//el que se esta buscando, si se encuentra lo devuelve
//y si no lo libera con free y pasa al siguiente, 
//si no se encunetra en toda la lista retorna NULL

char	*ft_get_path_command(char **command, char **env, char *path_temp)
{
	int		i;
	char	*path;
	char	**path_list;

	if (ft_strchr(command[0], '/'))
		if (access(command[0], X_OK) == 0)
			return (ft_strdup(command[0]));
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
		{
			path_list = ft_split(env[i], ':');
			path = ft_check_command(path_list, path_temp, command[0]);
			if (path)
				return (path);
		}
	}
	free_path_list(path_list);
	return (NULL);
}
//si un comando empieza por / significa que es una ruta absoluta
//luego comprueba que sea ejecutable y si lo es lo retorna
//Se recorre env (variables de entorno) buscando "PATH="
//se divide en directorios y se utiliza a la funcion de arriba para buscar el comando
//si existe retorna la ruta
//si no se encuentra el comando se libera memoria de path_list ny retorna NULL