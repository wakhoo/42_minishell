/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:43:27 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/20 01:05:40 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fd_handle(int *save, t_data *data)
{
	if (data->fd_in != STDIN_FILENO)
		ft_dup2(data->fd_in, STDIN_FILENO);
	if (data->fd_out != STDOUT_FILENO)
		ft_dup2(data->fd_out, STDOUT_FILENO);
	select_execute(data->sep, data);
	if (data->fd_in != STDIN_FILENO)
	{
		close(data->fd_in);
		data->fd_in = STDIN_FILENO;
	}
	if (data->fd_out != STDOUT_FILENO)
	{
		close(data->fd_out);
		data->fd_out = STDOUT_FILENO;
	}
	ft_dup2(save[0], STDIN_FILENO);
	ft_dup2(save[1], STDOUT_FILENO);
	return ((int)data->exitcode);
}

int	launch_execute_re(t_data *data, int start, int end)
{
	int	save[2];

	tab_to_list(data, start, end);
	if (!checkfds(data) || data->err_flag)
		return (1);
	else
		rmtokens(data);
	if (!data->token)
		return (0);
	if (data->sep)
		ft_free_tab(data->sep);
	data->sep = NULL;
	list_to_array(data, &data->sep);
	save[0] = dup(STDIN_FILENO);
	save[1] = dup(STDOUT_FILENO);
	return (fd_handle(save, data));
}

int	launch_execute(t_data *data, int start, int end)
{
	tab_to_list(data, start, end);
	if (data->sep)
		ft_free_tab(data->sep);
	data->sep = NULL;
	list_to_array(data, &data->sep);
	select_execute(data->sep, data);
	return ((int)data->exitcode);
}

int	launch_router(t_data *data, int start, int end, int index)
{
	if ((index == -1 && hasredirection(data->arg, start, end)))
		return (launch_execute_re(data, start, end));
	else
		return (launch_execute(data, start, end));
}

int	execute_subre(t_data *data, int start, int end, int reend)
{
	int		fd_save;
	int		check;

	fd_save = -1;
	check = -1;
	if (reend != -1)
	{
		tab_to_list_re(data, start, end, reend);
		if (!checkfds(data))
			return (-1);
		else
			rmtokens(data);
		if (data->fd_out != STDOUT_FILENO)
		{
			fd_save = dup(STDOUT_FILENO);
			dup2(data->fd_out, STDOUT_FILENO);
			close(data->fd_out);
		}
	}
	if (fd_save != -1 && reend != -1)
		check = fd_save;
	return (execute_sub(data, start, end, check));
}

// void	print_data(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (data->sep[i] != NULL)
// 	{
// 		printf("arg[%d]:%s\n", i, data->sep[i]);
// 		i++;
// 	}
// }