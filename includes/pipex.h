/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:26:41 by ksudyn            #+#    #+#             */
/*   Updated: 2025/02/11 12:26:42 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/wait.h>


int	ft_error(int i);

int	main(int argc, char **argv, char **env);

int	ft_pipex(int *fd, char **kid_1, char **kid_2, char **env);

char	*ft_check_command(char **path_list, char *path_mid, char *kid);
char	*ft_get_path_command(char **kid, char **env, char *path_mid);

void	free_kids(char **kid);

#endif