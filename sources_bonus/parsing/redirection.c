/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 00:16:06 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/14 15:14:27 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_fd(char *filename, char *symbol, int checker, int *fd)
{
	int		flag;

	if (checker == 1)
	{
		flag = O_WRONLY | O_CREAT;
		if (!ft_strcmp(symbol, ">>"))
			flag |= O_APPEND;
		else
			flag |= O_TRUNC;
		*fd = open(filename, flag, 0644);
	}
	else
	{
		if (check_access(filename))
		{
			*fd = -1;
			return ;
		}
		*fd = open(filename, O_RDONLY);
	}
}

int	redirection(t_data *data, t_list *cont)
{
	int		fd;

	fd = 0;
	if (!ft_strcmp(cont->content, ">") || !ft_strcmp(cont->content, ">>"))
	{
		get_fd(cont->next->content, cont->content, 1, &fd);
		update_fd(&data->fd_out, fd, STDOUT_FILENO);
	}
	else if (!ft_strcmp(cont->content, "<"))
	{
		get_fd(cont->next->content, cont->content, 0, &fd);
		update_fd(&data->fd_in, fd, STDIN_FILENO);
	}
	if (data->fd_in == -1 || data->fd_out == -1)
	{
		data->err_flag = 1;
		return (0);
	}
	return (1);
}

int	current_tmp(t_data *data)
{
	int		fd;

	fd = 0;
	while (data->cnt_here != 0)
	{
		data->tmps = data->tmps->next;
		data->cnt_here--;
	}
	if (data->tmps != NULL)
	{
		fd = open(data->tmps->content, O_RDONLY);
		if (fd != -1)
			update_fd(&data->fd_in, fd, STDIN_FILENO);
		else if (fd == -1)
		{
			data->exitcode = 1;
			exit_minishell("Error:\nopen failed", -42, data);
		}
		data->tmps = data->tmps->next;
	}
	return (fd);
}

bool	checkfds(t_data *data)
{
	t_list	*list[3];

	list[0] = data->token;
	list[1] = NULL;
	list[2] = NULL;
	while (list[0] != NULL)
	{
		if (checksymbol(list[0]->content) > 0
			&& checksymbol(list[0]->content) < 4)
		{
			if (!ft_strcmp(list[0]->content, "<<") && !data->err_flag)
				if (current_tmp(data) == -1)
					return (0);
			if (list[0]->next != NULL && checksymbol(list[0]->content) == 1)
				list[1] = list[0];
			if (list[0]->next != NULL && checksymbol(list[0]->content) == 2)
				list[2] = list[0];
		}
		list[0] = list[0]->next;
	}
	if ((list[1] != NULL && !redirection(data, list[1]))
		|| (list[2] != NULL && !redirection(data, list[2])))
		return (0);
	return (1);
}
//line[0] = data->token / line[1] = last fd_out /line[2] = last fd_in
