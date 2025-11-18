/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:21:49 by dancel            #+#    #+#             */
/*   Updated: 2025/02/13 18:48:02 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exitcode_convert(int exitcode, t_data *data)
{
	int	i;

	i = 0;
	if (exitcode == -42)
		exitcode = data->exitcode % 256;
	if (!data->sep || !data->sep[0] || ft_strcmp(data->sep[0], "exit"))
		return (printf("exit\n"), exitcode % 256);
	if (!ft_strcmp(data->sep[0], "exit") && !data->sep[1])
		exitcode = data->exitcode % 256;
	else if (!ft_strcmp(data->sep[0], "exit") && data->sep[1])
	{
		while (data->sep[1][++i])
		{
			if (!(data->sep[1][0] == '-' || ft_isdigit(data->sep[1][0]))
			|| !ft_isdigit(data->sep[1][i]))
				return (printf \
				("minishell: exit: %s: numeric argument required\n", \
				data->sep[1]), 1);
		}
		if (data->sep[2])
			return (printf("exit\nminishell: exit: too many arguments\n"), \
			data->exitcode = 1, -420);
		return (printf("exit\n"), ft_atoll(data->sep[1]) % 256);
	}
	return (printf("exit\n"), exitcode % 256);
}

void	exit_minishell(char *error_msg, int exitcode, t_data *data)
{
	exitcode = exitcode_convert(exitcode, data);
	if (exitcode == -420)
		return ;
	unlink_tmps(data);
	ft_free_tab(data->arg);
	ft_free_tab(data->sep);
	rl_clear_history();
	ft_lstclear(&data->cnvp, free);
	ft_lstclear(&data->token, free);
	free(data);
	write(2, error_msg, ft_strlen(error_msg));
	exit(exitcode);
}
