/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:46:53 by ksudyn            #+#    #+#             */
/*   Updated: 2025/06/20 17:49:14 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	return_fd_in(t_list *node)
{
	int	i;
	int	last_index;

	i = 0;
	last_index = -1;
	while (node->redirecc[i])
	{
		if (ft_strcmp(node->redirecc[i], "<") == 0
			|| ft_strcmp(node->redirecc[i], "<<") == 0)
			last_index = i;
		i++;
	}
	return (last_index);
}

t_token_type	identify_reddir_in(t_list *node)
{
	int				i;
	t_token_type	result;

	i = 0;
	result = STD_IN;
	if (!node->redirecc)
	{
		if (node->prev)
			return (PIPE);
		else
			return (STD_IN);
	}
	while (node->redirecc[i])
	{
		if (ft_strcmp(node->redirecc[i], "<<") == 0)
			result = HEREDOC;
		else if (ft_strcmp(node->redirecc[i], "<") == 0)
			result = FD;
		i++;
	}
	if (result == STD_IN && node->prev)
		result = PIPE;
	return (result);
}

void	open_and_redir_in(t_list *node, t_execdata *exec, int i)
{
	int	fd;
	int	redir_type;

	redir_type = identify_reddir_in(node);
	if (redir_type == STD_IN)
		return ;
	if (redir_type == FD || redir_type == HEREDOC)
	{
		fd = open(node->fd[return_fd_in(node)], O_RDONLY);
		if (fd == -1)
		{
			perror(node->fd[return_fd_in(node)]);
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		return ;
	}
	if (redir_type == PIPE && i > 0)
	{
		dup2(exec->pipes[i - 1][0], STDIN_FILENO);
		close(exec->pipes[i - 1][1]);
		close(exec->pipes[i - 1][0]);
		return ;
	}
}
