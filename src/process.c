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

char	*ft_check_command(char **path_list, char *path_temp, char *kid)
{
	int		i;
	int		j;
	char	*path_result;

	i = -1;
	while (path_list[++i])
	{
		path_temp = ft_strjoin(path_list[i], "/");
		path_result = ft_strjoin(path_temp, kid);
		free(path_temp);
		if (access(path_result, X_OK) == 0)
		{
			j = 0;
			while (path_list[j])
			{
				free(path_list[j]);
				j++;
			}
			free(path_list);
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

char	*ft_get_path_command(char **kid, char **env, char *path_mid)
{
	int		i;
	char	*path;
	char	**path_list;

	if (ft_strchr(kid[0], '/'))
		if (access(kid[0], X_OK) == 0)
			return (ft_strdup(kid[0]));
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp("PATH=", env[i], 5))
		{
			path_list = ft_split(env[i], ':');
			path = ft_check_command(path_list, path_mid, kid[0]);
			if (path)
				return (path);
		}
	}
	i = 0;
	while (path_list[i])
	{
		free(path_list[i]);
		i++;
	}
	return (free(path_list), NULL);
}
