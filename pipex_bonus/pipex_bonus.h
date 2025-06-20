/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:26:35 by ksudyn            #+#    #+#             */
/*   Updated: 2025/06/20 17:51:52 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
#define PIPEX_BONUS_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

# define ARGC_ERROR 1

typedef struct s_execdata
{
	int	cmd_index;
	int	total_cmds;
	int	infile;
	int	outfile;
	int	heredoc;
	int **pipes;
	pid_t *pids;
} t_execdata;

typedef enum e_token_type
{
	STD_IN,
	FD,
	HEREDOC,
	PIPE
}	t_token_type;

typedef struct s_list {
	char	**cmd;          // array del comando y args
	char	*cmd_path;      // ruta completa
	char	**redirecc;     // array de redirecciones tipo "<", ">", "<<"
    char       **fd;
	struct s_list *next;
    struct s_list *prev;
} t_list;


// execute_bonus.c
int execute_pipeline(int argc, char **argv, char **envp);
void execute_child(char **argv, char **envp, t_execdata *data);

// heredoc.c
int execute_here_doc(int argc, char **argv, char **envp);
int get_heredoc_input(const char *limiter);

// utils_bonus.c
char *ft_get_path_command(char **command, char **env, char *path_temp);
char *ft_check_command(char **path_list, char *path_temp, char *command);
void free_kids(char **kid);
void free_path_list(char **path_list);
void ft_error_exit(const char *msg);
char **ft_split(const char *str, char c);
char *ft_strjoin(const char *s1, const char *s2);
int ft_strcmp(const char *s1, const char *s2);

#endif
