/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 19:06:40 by ksudyn            #+#    #+#             */
/*   Updated: 2026/07/07 17:19:15 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static int	open_outfile(char *filename, int heredoc)
{
	int	fd;

	// Si venimos de un here_doc, abre el archivo final en "modo añadir" (O_APPEND)
	// para escribir al final de lo que ya hubiera dentro sin borrar nada.
	if (heredoc)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// Si venimos del modo normal, vacía el archivo por completo (O_TRUNC)
	// para escribir la nueva información totalmente desde cero.
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

static int	init_infile(char **argv, int heredoc)
{
	int	fd;

	// Si el usuario pidió un here_doc, le pasa la responsabilidad 
	// a la función especializada en capturar el teclado.
	if (heredoc)
		fd = handle_heredoc(argv[2]);
	// Si no es un heredoc, intenta abrir el archivo normal de entrada.
	else
	{
		fd = open(argv[1], O_RDONLY);
		// Si el archivo no existe o no se puede abrir, escribe el error
		// y sale inmediatamente (exit 1).
		if (fd < 0)
		{
			write(2, "pipex: input file error\n", 24);
			exit(1);
		}
	}
	return (fd);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		heredoc;

	// Revisa que haya suficientes argumentos mínimos en la terminal.
	if (argc < 5)
		return (write(2, "Error: Argumentos insuficientes.\n", 33), 1);
	heredoc = 0;
	// Detecta si el usuario ha escrito "here_doc".
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		// Exige los 6 argumentos necesarios para que el flujo tenga sentido.
		if (argc < 6)
			return (write(2, "Error: Estructura heredoc invalida.\n", 36), 1);
		heredoc = 1;
	}
	// Abre las llaves de paso de los archivos de entrada y de salida.
	pipex.in_fd = init_infile(argv, heredoc);
	pipex.out_fd = open_outfile(argv[argc - 1], heredoc);
	// Si el archivo de salida falló, cierra la entrada y cancela todo.
	if (pipex.out_fd < 0)
	{
		if (pipex.in_fd >= 0)
			close(pipex.in_fd);
		return (write(2, "Error: Outfile\n", 15), 1);
	}
	// Si todo está en orden, arranca la cadena de montaje de comandos.
	run_pipeline(argc, argv, envp, &pipex);
	return (0);
}
