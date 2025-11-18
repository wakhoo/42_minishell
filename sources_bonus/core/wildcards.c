/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:23:27 by dancel            #+#    #+#             */
/*   Updated: 2025/02/14 14:53:45 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_between(char *file, char *arg)
{
	int		i;

	i = -1;
	if (!ft_strsuffix(file, ft_strchr(arg, '*') + 1))
		return (0);
	while (file [++i] && arg[i] != '*')
	{
		if (file[i] != arg[i])
			return (0);
	}
	return (1);
}

static void	replace_arg(char *arg, char *new, int mod, DIR *dir)
{
	struct dirent	*entry;
	int				i;

	entry = readdir(dir);
	i = 0;
	while (entry)
	{
		if ((mod == SUFFIX && ft_strsuffix(entry->d_name, arg + 1)) \
		|| (mod == PREFIX && ft_strprefix(entry->d_name, arg)) \
		|| (mod == ALL) \
		|| (mod == WORD && ft_strstr(entry->d_name, arg + 1)) \
		|| (mod == INSIDE && is_between(entry->d_name, arg)))
		{
			if (!ft_strprefix(entry->d_name, "."))
			{
				i += ft_strcpy(new + i, entry->d_name);
				new[i] = ' ';
				i++;
			}
		}
		entry = readdir(dir);
	}
	new[i] = '\0';
}

static int	find_mod(char *arg, int mod)
{
	if (arg[0] == '*')
		mod = SUFFIX;
	if (arg[0] && arg[ft_strlen(arg) - 1] == '*')
	{
		if (mod == SUFFIX && !arg[1])
			mod = ALL;
		else if (mod == SUFFIX)
		{
			mod = WORD;
			arg[ft_strlen(arg) - 1] = '\0';
		}
		else
		{
			mod = PREFIX;
			arg[ft_strlen(arg) - 1] = '\0';
		}
	}
	if (ft_strchr(arg, '*') && mod == 0)
		mod = INSIDE;
	if ((mod != WORD && ft_strchrcounter(arg, '*') > 1)
		|| (mod == WORD && ft_strchrcounter(arg, '*') > 2))
		mod = 0 ;
	return (mod);
}

static void	wildcards_token(DIR *dir, t_list **current, \
							t_list **previous, t_data *data)
{
	char	new[BUFFER_SIZE];
	int		mod;

	mod = find_mod(((char *)(*current)->content), 0);
	if (!mod)
	{
		*previous = *current;
		*current = (*current)->next;
		return ;
	}
	replace_arg(((char *)(*current)->content), new, mod, dir);
	if (new[0] != '\0')
	{
		connect_list(data, previous, new, -1);
		if (*previous != NULL && (*previous)->next != NULL)
			*current = (*previous)->next;
		else
			*current = NULL;
	}
	else
	{
		*previous = *current;
		*current = (*current)->next;
	}
}

void	wildcards(t_data *data)
{
	char	cwd[BUFFER_SIZE];
	DIR		*dir;
	t_list	*current;
	t_list	*previous;

	current = data->token;
	previous = NULL;
	dir = opendir(getcwd(cwd, BUFFER_SIZE));
	if (!dir)
		return (error_message("dir", errno, data));
	while (current)
		wildcards_token(dir, &current, &previous, data);
	closedir(dir);
}
