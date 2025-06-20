/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:26:13 by ksudyn            #+#    #+#             */
/*   Updated: 2025/06/20 17:42:56 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_herdocs(t_list *list, t_execdata *exec)
{
	t_list	*current;
	int		i;
	char	*temp;
	char	*temp_2;

	current = list;
	temp = NULL;
	temp_2 = NULL;
	i = 0;
	while (i <= exec->heredoc)
	{
		temp_2 = ft_itoa(i);
		temp = ft_strjoin("temp_here_", temp_2);
		if (access(temp, F_OK) != -1)
			unlink(temp);
		free(temp);
		free(temp_2);
		i++;
	}
}

int	comprove_heredocs(t_list *list)
{
	t_list	*current;

	current = list;
	while (current)
	{
		if (have_a_heredoc(current) == 1)
		{
			if (open_all_herdocs(current) == -1)
			{
				return (-1);
			}
			current = current->next;
		}
		else
			current = current->next;
	}
	return (0);
}

void	close_herdocs(t_list *list, t_execdata *exec)
{
	t_list	*current;
	int		i;
	char	*temp;
	char	*temp_2;

	current = list;
	temp = NULL;
	temp_2 = NULL;
	i = 0;
	while (i <= exec->heredoc)
	{
		temp_2 = ft_itoa(i);
		temp = ft_strjoin("temp_here_", temp_2);
		if (access(temp, F_OK) != -1)
			unlink(temp);
		free(temp);
		free(temp_2);
		i++;
	}
}
