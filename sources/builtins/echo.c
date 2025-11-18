/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 21:33:36 by dancel            #+#    #+#             */
/*   Updated: 2025/02/06 16:36:57 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	n_counter(t_data *data)
{
	int	i;
	int	j;
	int	n;

	i = 0;
	n = 0;
	while (data->sep[++i])
	{
		if (data->sep[i][0] == '-' && data->sep[i][1])
		{
			j = 0;
			while (data->sep[i][j + 1] == 'n')
				j++;
			if (!data->sep[i][j + 1])
				n++;
			else
				return (n);
		}
		else
			return (n);
	}
	return (n);
}

void	echo(t_data *data)
{
	int	i;
	int	n;

	i = 0;
	n = n_counter(data);
	while (data->sep[++i + n])
	{
		printf("%s", data->sep[i + n]);
		if (data->sep[i + n + 1])
			printf(" ");
	}
	if (n == 0)
		printf("\n");
	data->exitcode = 0;
}
