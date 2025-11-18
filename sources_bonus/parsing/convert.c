/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 01:03:40 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/10 14:17:47 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	convert_envp(char *s, t_data *data)
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		exit_minishell("Error:\nmalloc failed", -42, data);
	new_node->content = ft_strdup(s);
	if (!new_node->content)
	{
		free(new_node);
		exit_minishell("Error:\nmalloc failed", -42, data);
	}
	new_node->next = NULL;
	if (data->cnvp == NULL)
		data->cnvp = new_node;
	else
		ft_lstlast(data->cnvp)->next = new_node;
}

static void	get_node(char *line, t_data *data, int *start, int *cnt)
{
	char	*str;

	if (*cnt != 0)
	{
		str = ft_substr(line, *start, *cnt);
		if (!str)
			exit_minishell("Error:\nmalloc failed", -42, data);
		token_node(data, str);
		free(str);
		str = NULL;
		*start += *cnt;
		*cnt = 0;
	}
}

void	char_to_list(char *line, t_data *data, int start)
{
	int	cnt;

	cnt = 0;
	while (line[start] != '\0' && start <= (int)ft_strlen(line))
	{
		cnt = check_meta(line, start);
		get_node(line, data, &start, &cnt);
		if (line[start] == '\'' || line[start] == '"')
		{
			cnt = n_quotes(line, start);
			get_node(line, data, &start, &cnt);
		}
		cnt = cnt_word(line, start);
		get_node(line, data, &start, &cnt);
		cnt = pass_space(line, start);
		start += cnt;
		cnt = 0;
	}
}

void	list_to_array(t_data *data, char ***tab)
{
	int		size;
	int		i;
	t_list	*first;

	first = data->token;
	size = ft_lstsize(data->token);
	*tab = malloc(sizeof(char *) * (size + 1));
	if (!*tab)
		exit_minishell("Error:\nmalloc failed", -42, data);
	i = 0;
	while (data->token != NULL)
	{
		(*tab)[i] = ft_strdup((char *)data->token->content);
		if (!(*tab)[i])
		{
			while (i >= 0)
				free((*tab)[i--]);
			free(*tab);
			exit_minishell("Error:\nmalloc failed", -42, data);
		}
		data->token = data->token->next;
		i++;
	}
	(*tab)[i] = NULL;
	data->token = first;
}
