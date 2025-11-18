/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:09:38 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/11 22:04:03 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lowest_op(char **tab, int start, int end)
{
	int	p_lv;

	p_lv = 0;
	while (tab[start] && start <= end)
	{
		if (!ft_strcmp(tab[start], "("))
			p_lv += 1;
		else if (!ft_strcmp(tab[start], ")"))
			p_lv -= 1;
		else if (p_lv == 0 && (!ft_strcmp(tab[start], "&&")
				|| !ft_strcmp(tab[start], "||")))
			return (start);
		start ++;
	}
	return (-1);
}

static int	reindex(char **tab, int save, int end)
{
	while (tab[save] && save <= end)
	{
		if (checksymbol(tab[save]) == 4 || checksymbol(tab[save]) == 5
			|| !ft_strcmp(tab[save], "("))
			break ;
		save++;
	}
	return (save);
}

int	diff_op(char **tab, int start, int end, char *op)
{
	int	p_lv;

	p_lv = 0;
	while (tab[start] && start <= end)
	{
		if (!ft_strcmp(tab[start], "("))
			p_lv += 1;
		else if (!ft_strcmp(tab[start], ")"))
			p_lv -= 1;
		else if (p_lv == 0 && (!ft_strcmp(tab[start], "&&")
				|| !ft_strcmp(tab[start], "||")) && ft_strcmp(tab[start], op))
			return (start);
		start ++;
	}
	return (-1);
}

int	check_redirec(char **tab, int start, int end, int p_lv)
{
	int	save;

	save = -2;
	while (tab[start] && start <= end)
	{
		if (!ft_strcmp(tab[start], "("))
			p_lv += 1;
		else if (!ft_strcmp(tab[start], ")"))
		{
			p_lv -= 1;
			save = start;
		}
		else if (start == save + 1 && checksymbol(tab[start]) >= 1
			&& checksymbol(tab[start]) < 4 && p_lv == 0)
		{
			save = save + 2;
			return (reindex(tab, save, end));
		}
		start++;
	}
	return (-1);
}

int	right_res(t_data *data, int end, int index, int left_res)
{
	if (!ft_strcmp(data->arg[index], "&&"))
	{
		if (left_res == 0)
			return (execute_total(data, index + 1, end));
		else
		{
			index = diff_op(data->arg, index + 1, end, "&&");
			if (index != -1 && !ft_strcmp(data->arg[index], "||"))
				return (execute_total(data, index + 1, end));
			return (1);
		}
	}
	if (!ft_strcmp(data->arg[index], "||"))
	{
		if (left_res != 0)
			return (execute_total(data, index + 1, end));
		else
		{
			index = diff_op(data->arg, index + 1, end, "||");
			if (index != -1 && !ft_strcmp(data->arg[index], "&&"))
				return (execute_total(data, index + 1, end));
			return (0);
		}
	}
	return (-1);
}
