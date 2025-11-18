/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:21:05 by dancel            #+#    #+#             */
/*   Updated: 2025/02/14 15:50:26 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_line(char	*var)
{
	char	*value;
	char	*cpy;

	cpy = ft_strdup(var);
	value = ft_strchr(cpy, '=');
	if (var[0] == '_' && var[1] == '=')
	{
		free(cpy);
		return ;
	}
	if (value)
	{
		*value = '\0';
		value++;
		printf("declare -x %s=\"%s\"\n", cpy, value);
	}
	else
		printf("declare -x %s\n", cpy);
	free(cpy);
}

void	print_export(t_data *data)
{
	int		i;
	char	*var;
	char	*min;
	t_list	*current;

	min = " ";
	i = 0;
	while (++i <= ft_lstsize(data->cnvp))
	{
		current = data->cnvp;
		var = "~";
		while (current)
		{
			if (ft_strcmp(var, current->content) > 0
				&& ft_strcmp(current->content, min) > 0)
				var = current->content;
			current = current->next;
		}
		min = var;
		print_export_line(var);
	}
	data->exitcode = 0;
}

static int	is_right_format(char *arg)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (0);
	while (arg[++i] && arg[i] != '=' && (arg[i] != '+' && arg[i] != '='))
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
	}
	return (1);
}

void	export_var(char *var, char *value, char *arg, t_data *data)
{
	t_list	*current;
	char	*temp;

	current = ft_lstchr(data->cnvp, var);
	if ((!current || !current->content) && arg[ft_strlen(var)] != '+')
		ft_lstadd_back(&data->cnvp, (ft_lstnew(arg)));
	else if (current && current->content)
	{
		if (!ft_strchr(arg, '='))
			return ;
		temp = ft_strdup(current->content);
		if (!temp)
			exit_minishell("Error:\nmalloc failed", -42, data);
		free(current->content);
		if (arg[ft_strlen(var)] == '+')
			current->content = ft_strjoin(temp, value);
		else if (ft_strchr(arg, '=') && !(*ft_strchr(arg, '=') + 1))
			current->content = ft_strjoin(var, "=");
		else
			current->content = ft_strdup(arg);
		free(temp);
		if (!current->content)
			exit_minishell("Error:\nmalloc failed", -42, data);
	}
}

void	export(int i, t_data *data)
{
	char	*value;
	char	*var;

	if (!data->sep[1])
		print_export(data);
	while (data->sep[++i])
	{
		if (!is_right_format(data->sep[i]))
		{
			printf("export: `%s': not a valid identifier\n", data->sep[i]);
			data->exitcode = 1;
			continue ;
		}
		var = ft_strdup(data->sep[i]);
		if (!var)
			exit_minishell("Error:\nmalloc failed", -42, data);
		value = ft_strchr(var, '=');
		if (value)
			value[0] = '\0';
		if (ft_strchr(var, '+'))
			ft_strchr(var, '+')[0] = '\0';
		export_var(var, value + 1, data->sep[i], data);
		free(var);
		data->exitcode = 0;
	}
}
