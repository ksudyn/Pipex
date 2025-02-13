/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksudyn <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:26:18 by ksudyn            #+#    #+#             */
/*   Updated: 2025/02/11 12:26:21 by ksudyn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_error(int i)
{
	if (i == 1)
		write(2, "Error. No hay suficientes argumentos.\n", 37);
	if (i == 2)
		write(2, "Error. Archivos inválidos.\n", 28);
	return (1);
}
