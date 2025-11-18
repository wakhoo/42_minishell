/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 21:36:46 by dancel            #+#    #+#             */
/*   Updated: 2025/02/17 16:51:15 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char *var, t_data *data)
{
	if (!ft_lstchr(data->cnvp, var))
		return (NULL);
	return (ft_strchr(ft_lstchr(data->cnvp, var)->content, '=') + 1);
}

void	export_edit(char *s, t_data *data)
{
	data->sep = malloc(sizeof(char *) * (3));
	data->sep[0] = ft_strdup("export");
	data->sep[1] = ft_strdup(s);
	data->sep[2] = NULL;
	export(0, data);
}

void	env(t_data *data)
{
	t_list	*current;

	if (data->sep && data->sep[1])
	{
		printf("env: '%s': No such file or directory\n", data->sep[1]);
		data->exitcode = 127;
		return ;
	}
	current = data->cnvp;
	while (current)
	{
		if (ft_strchr(current->content, '='))
			printf("%s\n", (char *)current->content);
		current = current->next;
	}
	data->exitcode = 0;
}
