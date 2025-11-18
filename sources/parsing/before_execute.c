/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   before_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:59:26 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/14 15:54:28 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*removeqs(char *str)
{
	bool	in_sq;
	bool	in_dq;
	int		j;
	int		i;
	char	*res;

	j = 0;
	i = 0;
	in_sq = false;
	in_dq = false;
	res = ft_calloc(ft_strlen(str), 1);
	if (!res)
		return (NULL);
	while (i < (int)ft_strlen(str))
	{
		if (str[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else if (str[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

void	rmquotes(t_data *data)
{
	t_list	*first;
	char	*save;

	first = data->token;
	while (data->token != NULL)
	{
		if (hasquotes(data->token->content))
		{
			save = removeqs(data->token->content);
			if (!save)
				exit_minishell("Error:\nmalloc failed", -42, data);
			free(data->token->content);
			data->token->content = NULL;
			data->token->content = ft_strdup(save);
			free(save);
			save = NULL;
			if (!data->token->content)
				exit_minishell("Error:\nmalloc failed", -42, data);
		}
		data->token = data->token->next;
	}
	data->token = first;
}

bool	heredoc(t_data *data, t_list *here)
{
	int	fd;

	if (here->next != NULL && checkdilm(here->next->content))
	{
		get_heredoc(here->next->content, data, &fd, 0);
		data->tmps = data->tmps_start;
	}
	else
	{
		if (here->next != NULL)
			heredoc_err(here->next->content);
		else
			heredoc_err(NULL);
		data->err_flag = 1;
		data->exitcode = 2;
		return (0);
	}
	return (1);
}

bool	heredocs(t_data *data)
{
	t_list	*curr;

	curr = data->token;
	while (curr != NULL)
	{
		if ((checksymbol(curr->content) > 0
				&& checksymbol(curr->content) < 4))
		{
			if (!ft_strcmp(curr->content, "<<"))
				if (!heredoc(data, curr))
					return (0);
			if (curr->next == NULL || (checksymbol(curr->next->content) > 0
					&& checksymbol(curr->next->content) < 4))
				return (error_check(curr, data));
		}
		if (checksymbol(curr->content) == 4 || checksymbol(curr->content) == 5)
			if (checksymbol(curr->next->content) == 4
				|| checksymbol(curr->next->content) == 5)
				return (error_check(curr, data));
		curr = curr->next;
	}
	return (1);
}

void	tokenize(char *line, t_data *data)
{
	int	cnt;

	cnt = 0;
	if (!check_quotes_and_space(line))
		return ;
	reset_data(data);
	if (p_close(line) == -1)
	{
		data->exitcode = 2;
		return ;
	}
	if (ft_iswhitespace(line[0]))
		cnt = pass_space(line, 0);
	char_to_list(line, data, cnt);
	if (ft_lstsize(data->token) == 0)
		return ;
	if (!heredocs(data))
		return ;
	change_env(data);
	rmquotes(data);
	wildcards(data);
	list_to_array(data, &data->arg);
	list_to_array(data, &data->sep);
}
