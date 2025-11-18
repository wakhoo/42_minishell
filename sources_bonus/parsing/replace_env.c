/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:37:04 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/13 17:05:49 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(char *str, int i, int keylen, t_data *data)
{
	char	*key;
	char	*res;
	char	*value;

	key = ft_substr(str, i + 1, keylen - 1);
	if (!key)
		exit_minishell("Error:\nmalloc failed", -42, data);
	value = ft_getenv(key, data);
	free(key);
	if (value == NULL)
		res = ft_strdup("");
	else
	{
		res = ft_strdup(value);
		if (!res)
			exit_minishell("Error:\nmalloc failed", -42, data);
	}
	return (res);
}

void	replace(t_data *data, char **tmp, int *i)
{
	int		keylen;
	char	*value;
	char	*res;

	keylen = cnt_n_keyname(*tmp, *i);
	value = get_value(*tmp, *i, keylen, data);
	if (!value)
	{
		free(*tmp);
		exit_minishell("Error:\nmalloc failed", -42, data);
	}
	(*tmp)[*i] = '\0';
	res = ft_sssjoin(*tmp, value, *tmp + (*i + keylen));
	*i = *i + ft_strlen(value);
	free(value);
	free(*tmp);
	if (!res)
		exit_minishell("Error:\nmalloc failed", -42, data);
	(*tmp) = ft_strdup(res);
	free(res);
	if (!*tmp)
		exit_minishell("Error:\nmalloc failed", -42, data);
}

void	repexitcode(t_data *data, char **tmp, int *i)
{
	int		keylen;
	char	*value;
	char	*res;

	update_exitcode(data, 0);
	value = ft_itoa(data->exitcode);
	if (!value)
	{
		free(*tmp);
		exit_minishell("Error:\nmalloc failed", -42, data);
	}
	keylen = ft_strlen(value);
	(*tmp)[*i] = '\0';
	res = ft_sssjoin(*tmp, value, *tmp + (*i + 2));
	*i = *i + keylen;
	free(value);
	free(*tmp);
	if (!res)
		exit_minishell("Error:\nmalloc failed", -42, data);
	(*tmp) = ft_strdup(res);
	free(res);
	if (!*tmp)
		exit_minishell("Error:\nmalloc failed", -42, data);
}

char	*key_to_val(t_data *data, char *str)
{
	int		i;
	bool	in_dq;
	bool	in_sq;	
	char	*tmp;

	i = 0;
	in_dq = false;
	in_sq = false;
	tmp = ft_strdup(str);
	if (!tmp)
		exit_minishell("Error:\nmalloc failed", -42, data);
	while (i < (int)ft_strlen(tmp))
	{
		if (tmp[i] == '"' && !in_sq)
			in_dq = !in_dq;
		if (tmp[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		if (tmp[i] == '$' && tmp[i + 1] != '?' && !in_sq)
			replace(data, &tmp, &i);
		else if (tmp[i] == '$' && tmp[i + 1] == '?' && !in_sq)
			repexitcode(data, &tmp, &i);
		else
			i++;
	}
	return (tmp);
}

void	change_env(t_data *data)
{
	t_list	*first;
	char	*save;

	first = data->token;
	while (data->token != NULL)
	{
		if (haveenv(data->token->content))
		{
			save = key_to_val(data, data->token->content);
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
