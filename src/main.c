/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:25:23 by ksudyn            #+#    #+#             */
/*   Updated: 2025/02/11 12:25:24 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv, char **env)
{
	int fd[2];  // fd[0] → infile, fd[1] → outfile
	char **command_1;
	char **command_2;
	int status;

	if (argc != 5)  // Verifica número de argumentos
		return (ft_error(1));

	// Abrir archivo de salida (outfile)
	fd[1] = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd[1] < 0)
	{
		close(fd[0]);//cierra el archivo de entrada
		//Porque si se produce un error al abrir el archivo de salida,
		//es una buena práctica cerrar cualquier archivo abierto previamente
		//para evitar fugas de recursos (en este caso, el archivo de entrada fd[0]).
		return (ft_error(2));
	}
	// Abrir archivo de entrada (infile)
	fd[0] = open(argv[1], O_RDONLY);
	if (fd[0] < 0)
		return (ft_error(2));

	// Separar comandos en listas de argumentos
	command_1 = ft_split(argv[2], ' ');
	command_2 = ft_split(argv[3], ' ');

	// Crear procesos y manejar pipes
	status = ft_pipex(fd, command_1, command_2, env);

	// Cerrar archivos y liberar memoria
	close(fd[0]);//Porque es una buena práctica cerrar los archivos después de que ya no se necesiten para liberar recursos
	close(fd[1]);//Para liberar el recurso y evitar fugas de memoria o descriptores de archivo abiertos innecesarios
	free_kids(command_1);
	free_kids(command_2);

	return (status);
}
//valgrind --leak-check=full --track-origins=yes ./pipex infile "ls -l" "wc -l" outfile
//para comprobar si no hay leaks de memoria
//Si te dice "All heap blocks were freed -- no leaks are possible", entonces todo está bien.
//el valgrind no sigue lo procesos hijos y puede no generase o modificar outfile
//entonces se hace esto./pipex infile "ls -l" "wc -l" outfile
//echo $?  # Verificar código de salida.
//Si el código de salida ($?) es diferente de 0, hay un error en la ejecución de algún comando
//o quitar del makefile  -fsanitize=address y entoces deberia comprobar porque ambos buscan leaks de memoria