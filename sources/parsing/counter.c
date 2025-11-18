/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 01:09:34 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/10 14:18:55 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cnt_n_keyname(char *str, int start)
{
	int	cnt;

	cnt = 1;
	while (str[start + cnt] != '\0'
		&& (ft_isalnum(str[start + cnt]) || str[start + cnt] == '_'))
		cnt++;
	return (cnt);
}

int	cnt_word(char *str, int start)
{
	int	cnt;
	int	qcnt;

	cnt = 0;
	qcnt = 0;
	while (!ft_iswhitespace(str[start + cnt])
		&& !check_meta(str, start + cnt) && str[start + cnt] != '\0')
	{
		if (str[start + cnt] == '\'' || str[start + cnt] == '"')
		{
			qcnt = n_quotes(str, start + cnt);
			cnt += qcnt;
			qcnt = 0;
		}
		else
			cnt++;
	}
	return (cnt);
}

int	pass_space(char *str, int i)
{
	int	cnt;

	cnt = 0;
	while (ft_iswhitespace(str[i + cnt]) && str[i + cnt] != '\0')
		cnt++;
	return (cnt);
}

int	n_quotes(char *str, int i)
{
	char	quote;
	int		cnt;

	cnt = 1;
	quote = str[i];
	while (str[i + cnt] != quote && str[i + cnt] != '\0')
		cnt++;
	if (str[i + cnt] == quote)
		return (cnt + 1);
	return (0);
}

int	p_close(char *str)
{
	int	open;
	int	i;

	i = 0;
	open = 0;
	while (str[i])
	{
		if (str[i] == '(')
			open++;
		if (str[i] == ')')
			open--;
		i++;
	}
	if (open != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		if (open < 0)
			ft_putstr_fd("syntax error near unexpected token `)'\n", 2);
		else if (open > 0)
			ft_putstr_fd("syntax error near unexpected token `('\n", 2);
		return (-1);
	}
	return (0);
}
