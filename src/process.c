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

char	*ft_check_command(char **path_list, char *path_mid, char *kid)
{
	int		j;
	int		i;
	char	*path_fin;

	j = -1;
	while (path_list[++j])
	{
		path_mid = ft_strjoin(path_list[j], "/");
		path_fin = ft_strjoin(path_mid, kid);
		free(path_mid);
		if (access(path_fin, X_OK) == 0)
		{
			i = 0;
			while (path_list[i])
			{
				free(path_list[i]);
				i++;
			}
			free(path_list);
			return (path_fin);
		}
		free(path_fin);
	}
	return (NULL);
}

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
