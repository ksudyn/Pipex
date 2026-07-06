/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:26:41 by ksudyn            #+#    #+#             */
/*   Updated: 2026/07/06 21:08:03 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../Libft/libft.h"
# include <fcntl.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

int		ft_error(int i);

int		main(int argc, char **argv, char **env);

int		ft_pipex(int *fd, char **kid_1, char **kid_2, char **env);

char	*ft_check_command(char **path_list, char *path_temp, char *kid);
char	*ft_get_path_command(char **kid, char **env, char *path_temp);

void	free_kids(char **kid);
void	free_path_list(char **path_list);

#endif
