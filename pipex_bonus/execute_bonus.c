/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:25:50 by ksudyn            #+#    #+#             */
/*   Updated: 2025/06/20 18:07:16 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	redir_and_exec(t_list *node, t_execdata *exec, t_list *list)
{
	open_and_redir_in(node, exec->cmd_index, exec);
	open_and_redir_out(node, exec->cmd_index, exec->total_cmds, exec);
	execute_node(node, exec, &list);
}


void	handle_child(t_list *node, t_execdata *exec, t_list *list)
{
	int	ret;

	close_unused_pipes(exec->cmd_index, exec->total_cmds, exec->pipes);
	ret = try_to_open_all_fds(node);
	if (ret == 0)
		redir_and_exec(node, exec, list);
	exit(1);
}


void	handle_parent(pid_t pid, t_execdata *exec, int *i)
{
	exec->pids[*i] = pid;
	(*i)++;
}


void	end_exec(t_list *list, t_execdata *exec, int i)
{
	close_all_pipes(exec->total_cmds, exec);
	wait_all_processes(exec, i);
	close_herdocs(list, exec);
}


void	execute_list(t_list *list, t_execdata exec)
{
	t_list	*cur;
	int		i;
	pid_t	pid;

	cur = list;
	i = 0;
	exec.cmd_index = 0;
	exec.total_cmds = list_size(&list);
	init_exec_data(&list, &exec);
	while (cur)
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
			handle_child(cur, &exec, list);
		handle_parent(pid, &exec, &i);
		cur = cur->next;
		if (cur)
			exec.cmd_index++;
	}
	end_exec(list, &exec, i);
}


