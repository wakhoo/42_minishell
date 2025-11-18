/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:25:53 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/14 14:54:09 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	checkdilm(char *str)
{
	char	*check;
	int		i;

	check = "#|<>&();";
	i = -1;
	while (check[++i])
	{
		if (str[0] == check[i])
			return (0);
	}
	return (1);
}

int	check_meta(char *str, int i)
{
	int		j;
	char	*meta;

	j = 0;
	meta = "<>|&()";
	while (meta[j])
	{
		if (str[i] == meta[j])
		{
			if ((meta[j] != '(' && meta[j] != ')') && str[i + 1] == meta[j])
				return (2);
			else
				return (1);
		}
		j++;
	}
	return (0);
}

int	check_quotes_and_space(char *line)
{
	int	i;
	int	cnt;
	int	space;

	i = -1;
	cnt = 0;
	space = 0;
	while (i < (int)ft_strlen(line) && line[++i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			cnt = n_quotes(line, i);
			if (cnt == 0)
				return (0);
			i += cnt;
			cnt = 0;
		}
		if (ft_iswhitespace(line[i]))
			space++;
	}
	return (!(space == (int)ft_strlen(line)));
}

int	close_index(char **tab, int start, int end)
{
	int	open;
	int	flag;

	open = 0;
	flag = 0;
	while (tab[start] && start <= end)
	{
		if (!ft_strcmp(tab[start], "("))
		{
			open++;
			flag = 1;
		}
		if (!ft_strcmp(tab[start], ")"))
			open--;
		if (flag == 1 && open == 0)
			return (start);
		start++;
	}
	return (-1);
}
