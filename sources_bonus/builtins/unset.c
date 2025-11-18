/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:21:14 by dancel            #+#    #+#             */
/*   Updated: 2025/02/14 15:19:51 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_corpse(char *cpy, t_data *data)
{
	t_list	*current;
	t_list	*temp;
	int		c;

	current = data->cnvp;
	c = 1 + ft_lstsize(current) - ft_lstsize(ft_lstchr(current, cpy));
	while (c-- > 2)
		current = current->next;
	if (c == 0)
	{
		temp = data->cnvp;
		data->cnvp = data->cnvp->next;
		free(temp->content);
		free(temp);
		return ;
	}
	temp = current->next->next;
	free(current->next->content);
	free(current->next);
	current->next = temp;
}

void	unset(t_data *data)
{
	int		i;
	int		j;
	char	cpy[BUFFER_SIZE];

	i = 0;
	while (data->sep[++i])
	{
		j = -1;
		while (data->sep[i][++j])
			cpy[j] = data->sep[i][j];
		cpy[j] = '=';
		cpy[j + 1] = '\0';
		if (!ft_lstchr(data->cnvp, cpy))
		{
			cpy[j] = '\0';
			if (!ft_lstchr(data->cnvp, cpy)
				|| ft_strlen(ft_lstchr(data->cnvp, cpy)->content) > (size_t)j)
				continue ;
		}
		unset_corpse(cpy, data);
	}
	data->exitcode = 0;
}
