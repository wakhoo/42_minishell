/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 05:01:20 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/13 21:01:41 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_err(char *str)
{
	if (!str || str[0] == '#')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("'\n", 2);
	}
}

int	error_check(t_list *cur, t_data *data)
{
	if (cur->next == NULL || (cur->next != NULL
			&& checksymbol(cur->next->content)
			&& !ft_strcmp(cur->content, "|")))
	{
		if (checksymbol(cur->content) != 4 && checksymbol(cur->content) != 5)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		}
		else
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(cur->content, 2);
			ft_putstr_fd("'\n", 2);
		}
	}
	else if (cur->next != NULL && checksymbol(cur->next->content))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(cur->next->content, 2);
		ft_putstr_fd("'\n", 2);
	}
	data->err_flag = 1;
	data->exitcode = 2;
	return (0);
}

int	check_access(char *filename)
{
	if (access(filename, F_OK) == -1)
	{
		ft_putstr_fd("minishell: open: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": no such file or directory\n", 2);
		return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		ft_putstr_fd("minishell: open: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": permission denied\n", 2);
		return (1);
	}
	return (0);
}
