/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:13:36 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/13 18:40:23 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	rm_twotoken(t_data *data, t_list *here, t_list **pre)
{
	t_list	*next;

	next = here->next->next;
	ft_lstdelone(here->next, free);
	ft_lstdelone(here, free);
	if (next == NULL && pre == NULL)
		data->token = NULL;
	else if ((*pre) == NULL)
		data->token = next;
	else
		(*pre)->next = next;
}

void	rmtokens(t_data *data)
{
	t_list	*pre;
	t_list	*first;

	pre = NULL;
	first = data->token;
	while (first != NULL)
	{
		if (!ft_strcmp(first->content, "<<") || !ft_strcmp(first->content, ">>")
			|| !ft_strcmp(first->content, "<")
			|| !ft_strcmp(first->content, ">"))
		{
			rm_twotoken(data, first, &pre);
			if (pre == NULL)
				first = data->token;
			else
				first = pre;
			continue ;
		}
		else
		{
			pre = first;
			first = first->next;
		}
	}
}

void	token_node(t_data *data, char *s)
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
	if (data->token == NULL)
		data->token = new_node;
	else
		ft_lstlast(data->token)->next = new_node;
}

void	tmp_node(t_data *data, char *s)
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
	if (data->tmps == NULL)
	{
		data->tmps = new_node;
		data->tmps_start = data->tmps;
	}
	else
		ft_lstlast(data->tmps)->next = new_node;
}

void	tab_to_list(t_data *data, int start, int end)
{
	int	i;

	i = start;
	if (data->token != NULL)
		ft_lstclear(&data->token, free);
	data->token = NULL;
	while (data->arg[i] && (i <= end))
		token_node(data, data->arg[i++]);
}
