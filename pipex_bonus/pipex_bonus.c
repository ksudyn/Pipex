/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <ksudyn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:22:02 by ksudyn            #+#    #+#             */
/*   Updated: 2026/07/07 17:28:47 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	handle_heredoc(char *limiter)
{
	int		pipefd[2];
	char	*line;
	size_t	len;

	// Se crea el pipe para ir guardando lo que el usuario escriba
	if (pipe(pipefd) == -1)
		return (-1);
	len = ft_strlen(limiter);
	// Entra en un bucle infinito para leer líneas continuamente.
	while (1)
	{
		// Muestra el texto "heredoc> " en la pantalla para avisar al usuario.
		write(1, "heredoc> ", 9);
		// Lee la línea que escribe el usuario usando el get_next_line.
		line = get_next_line(0);
		if (!line)
			break ;
		// Compara: si lo escrito coincide exactamente con la palabra secreta (limiter)
		// y termina en un salto de línea (\n), limpia la memoria y ROMPE el bucle.
		if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
		{
			free(line);
			break ;
		}
		// Si no era la palabra secreta, inyecta el texto dentro del túnel (pipefd[1])
		// y borra la línea de la memoria para no acumular basura (leaks).
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	// Cuando termina, cierra el grifo de escritura del túnel.
	close(pipefd[1]);
	// Devuelve el extremo de lectura del túnel. Ahora el primer comando de Linux
	// podrá leer todo este texto como si estuviera leyendo de un archivo real.
	return (pipefd[0]);
}

static void	execute_bonus_cmd(char *cmd_arg, char **envp)
{
	char	**command;
	char	*path_command;

	// Trocea el comando y sus opciones (p.ej: "grep -i a" -> ["grep", "-i", "a"])
	command = ft_split(cmd_arg, ' ');
	if (!command)
		exit(1);
	// Busca en los caminos del sistema (envp) dónde está escondido el binario real (p.ej: /usr/bin/grep)
	path_command = ft_get_path_command(command, envp, NULL);
	// Si lo encuentra, ocurre ejecuta execve: el hijo "muere"
	// y su lugar lo ocupa el programa real de Linux.
	if (path_command)
		execve(path_command, command, envp);
	// Si el programa sigue ejecutándose aquí abajo, significa que el comando NO existía.
	// Escribe el error por pantalla, limpia la memoria y sale con el código estándar 127.
	write(2, "pipex: command not found\n", 25);
	free_kids(command);
	exit(127);
}

static void	child_process(t_pipex *pipex, char *cmd, char **envp, int is_last)
{
	// Conecta su lectura al tubo que le dejó preparado el comando anterior (prev_fd).
	dup2(pipex->prev_fd, STDIN_FILENO);
	// Conecta su salida: si es el ÚLTIMO comando,lo deja directo al archivo final (out_fd).
	if (is_last)
		dup2(pipex->out_fd, STDOUT_FILENO);
	// Si no es el último, conecta su salida a la escritura del túnel actual (pipefd[1]).
	else
		dup2(pipex->pipefd[1], STDOUT_FILENO);
	// Cierra las llaves de paso que ya ha redirigido para no dejar fugas de recursos.
	close(pipex->prev_fd);
	if (!is_last)
	{
		close(pipex->pipefd[0]);
		close(pipex->pipefd[1]);
	}
	close(pipex->out_fd);
	// Una vez cerrado todo, ejecuta el comando.
	execute_bonus_cmd(cmd, envp);
}

static void	parent_process(t_pipex *pipe, int i, int argc)
{
	// Cierra el pipe viejo que ya ha sido usado por el hijo.
	close(pipe->prev_fd);
	// Si todavía quedan más comandos por delante en la cadena:
	if (i < argc - 2)
	{
		// Cierra el pipe de escritura actual (porque solo escribirá el hijo).
		close(pipe->pipefd[1]);
		// Guarda la salida del pipe actual (pipefd[0]) como la entrada del próximo comando (prev_fd).
		pipe->prev_fd = pipe->pipefd[0];
	}
}

void	run_pipeline(int argc, char **argv, char **envp, t_pipex *pipex)
{
	int		i;
	pid_t	pid;

	// Por defecto, el primer comando real empieza en la posición 2 de los argumentos.
	i = 2;
	// Si usamos here_doc, salta una posición más porque el comando real empieza en la 3.
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		i = 3;
	// Define la primera fuente de datos del circuito como nuestro archivo de entrada (in_fd).
	pipex->prev_fd = pipex->in_fd;
	// Bucle principal: recorre y procesa los comandos uno tras otro.
	while (i < argc - 1)
	{
		// Si no es el último comando, abre un nuevo pipe para comunicar este comando con el siguiente.
		if (i < argc - 2 && pipe(pipex->pipefd) == -1)
			exit(1);
		// Crea un clon idéntico del programa (proceso hijo).
		pid = fork();
		// Si el resultado es 0, significa que somos el proceso HIJO. Ejecutamos el comando actual.
		if (pid == 0)
			child_process(pipex, argv[i], envp, (i == argc - 2));
		// El proceso PADRE hace el relevo de tuberías y pasa al siguiente comando (i++).
		parent_process(pipex, i, argc);
		i++;
	}
	// Cierra el archivo de salida final en el padre porque ya lo tienen los hijos.
	close(pipex->out_fd);
	// Se queda esperando pacientemente en la puerta (wait) a que todos los hijos terminen de trabajar.
	while (i-- > 0)
		wait(NULL);
}
