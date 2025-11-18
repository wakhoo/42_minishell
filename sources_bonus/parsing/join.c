/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:32:00 by chajeon           #+#    #+#             */
/*   Updated: 2025/01/23 10:33:15 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcjoin(char *str, int c)
{
	char	*dest;
	int		i;

	i = 0;
	if (!str || c == '\0')
		return (NULL);
	dest = malloc(ft_strlen(str) + 2);
	if (dest == NULL)
		return (NULL);
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i + 0] = c;
	dest[i + 1] = '\0';
	return (dest);
}

char	*ft_sssjoin(char *str1, char *str2, char *str3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(str1, str2);
	res = ft_strjoin(tmp, str3);
	free(tmp);
	return (res);
}
