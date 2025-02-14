/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:25:46 by ksudyn            #+#    #+#             */
/*   Updated: 2025/02/11 12:25:48 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_kids(char **kid)
{
	int	i;

	i = 0;
	while (kid[i])
	{
		free(kid[i]);
		i++;
	}
	free(kid);
}

void	free_path_list(char **path_list)
{
	int	i;

	i = 0;
	if (!path_list)
		return ;
	while (path_list[i])
	{
		free(path_list[i]);
		i++;
	}
	free(path_list);
}
