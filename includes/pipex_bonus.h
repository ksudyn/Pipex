/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:26:35 by ksudyn            #+#    #+#             */
/*   Updated: 2026/07/06 21:13:32 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../Libft/libft.h"
# include "pipex.h"

typedef struct s_pipex
{
	int	in_fd;
	int	out_fd;
	int	prev_fd;
	int	pipefd[2];
}		t_pipex;

/* Lógica de procesamiento de bonus */
int		handle_heredoc(char *limiter);
void	run_pipeline(int argc, char **argv, char **envp, t_pipex *boss);

#endif
