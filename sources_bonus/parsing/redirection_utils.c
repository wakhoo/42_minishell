/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:44:36 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/14 14:47:27 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_fd(int *fd, int filenum, int checker)
{
	if (*fd != checker)
		close(*fd);
	*fd = filenum;
}

int	checksymbol(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>"))
		return (1);
	if (!ft_strcmp(str, "<"))
		return (2);
	if (!ft_strcmp(str, "<<"))
		return (3);
	if (!ft_strcmp(str, "|"))
		return (4);
	if (!ft_strcmp(str, "||") || !ft_strcmp(str, "&&"))
		return (5);
	if (!ft_strcmp(str, "&"))
		return (6);
	return (0);
}

bool	hasredirection(char **tab, int start, int end)
{
	while (tab[start] && start <= end)
	{
		if (checksymbol(tab[start]) > 0 && checksymbol(tab[start]) < 4)
			return (1);
		start++;
	}
	return (0);
}

void	tab_to_list_re(t_data *data, int start, int end, int reend)
{
	int	i;

	i = start;
	if (data->token != NULL)
		ft_lstclear(&data->token, free);
	data->token = NULL;
	while (data->arg[i] && (i <= reend))
	{
		if (i == end + 1)
		{
			i++;
			continue ;
		}
		token_node(data, data->arg[i++]);
	}
}
