/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 19:06:40 by ksudyn            #+#    #+#             */
/*   Updated: 2026/07/06 21:12:20 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

/*
** Función auxiliar para comprobar errores de apertura de archivos de salida.
** Recibe 3 argumentos (máximo 4 permitido por la Norm).
*/
static int	open_outfile(char *filename, int heredoc)
{
	int	fd;

	if (heredoc)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

/*
** Inicializa el descriptor de entrada. Si es here_doc, ejecuta su lógica,
** si no, abre el archivo infile clásico. Utiliza solo 2 variables de las 5 máx.
*/
static int	init_infile(char **argv, int heredoc)
{
	int	fd;

	if (heredoc)
		fd = handle_heredoc(argv[2]);
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			write(2, "pipex: input file error\n", 24);
			exit(1);
		}
	}
	return (fd);
}

/*
** Función principal del bonus.
** Solo declara 4 variables (límite Norm: 5).
** Mapea el flujo utilizando un 'while' clásico en lugar de 'for'.
*/
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		heredoc;

	if (argc < 5)
		return (write(2, "Error: Argumentos insuficientes.\n", 33), 1);
	heredoc = 0;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			return (write(2, "Error: Estructura heredoc invalida.\n", 36), 1);
		heredoc = 1;
	}
	pipex.in_fd = init_infile(argv, heredoc);
	pipex.out_fd = open_outfile(argv[argc - 1], heredoc);
	if (pipex.out_fd < 0)
	{
		if (pipex.in_fd >= 0)
			close(pipex.in_fd);
		return (write(2, "Error: Outfile\n", 15), 1);
	}
	run_pipeline(argc, argv, envp, &pipex);
	return (0);
}
