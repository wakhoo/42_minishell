/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_connect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:25:19 by dancel            #+#    #+#             */
/*   Updated: 2025/02/12 21:34:28 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wild_node(t_data *data, char *s, t_list **previous, int *check)
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
	if ((*previous) == NULL && *check == -1)
	{
		ft_lstdelone(data->token, free);
		(*previous) = new_node;
		data->token = (*previous);
		*check = 1;
	}
	else
	{
		(*previous)->next = new_node;
		(*previous) = (*previous)->next;
	}
}

void	connect_list(t_data *data, t_list **list, char *new, int check)
{
	t_list	*next;
	char	**newtab;
	int		i;

	next = NULL;
	if ((*list) != NULL)
	{
		next = ((*list)->next->next);
		if ((*list)->next != NULL)
			ft_lstdelone((*list)->next, free);
	}
	newtab = NULL;
	newtab = ft_split(new, ' ');
	if (!newtab)
		exit_minishell("Error:\nmalloc failed", -42, data);
	i = 0;
	while (newtab[i])
	{
		wild_node(data, newtab[i], list, &check);
		i++;
	}
	if (next != NULL)
		(*list)->next = next;
	ft_free_tab(newtab);
}
