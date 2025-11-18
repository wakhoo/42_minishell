/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:33:45 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/20 18:15:10 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_shlvl(t_data *data)
{
	char	*temp1;
	char	*temp2;

	if (ft_lstchr(data->cnvp, "SHLVL=") && ft_atoi(ft_getenv("SHLVL=", data)) \
		< 999 && ft_strisdigit(ft_getenv("SHLVL=", data)))
	{
		temp1 = ft_itoa(ft_atoi(ft_getenv("SHLVL=", data)) + 1);
		temp2 = ft_strjoin("SHLVL=", temp1);
		export_edit(temp2, data);
		if (!data->sep[0] || !data->sep[1])
			exit_minishell("Error:\nmalloc failed", -42, data);
		free(temp1);
		free(temp2);
	}
	else
	{
		export_edit("SHLVL=1", data);
		if (!data->sep[0] || !data->sep[1])
			exit_minishell("Error:\nmalloc failed", -42, data);
		if (data->sep)
			ft_free_tab(data->sep);
	}
}

void	init_data(char **envp, t_data *data)
{
	char	*tmp;
	int		i;

	i = -1;
	*data = (t_data){0};
	while (envp[++i])
		convert_envp(envp[i], data);
	if (!ft_lstchr(data->cnvp, "PATH="))
	{
		tmp = ft_strjoin("PATH=", "/home/dancel/.local/funcheck/host:/hom\
			e/dancel/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbi\
			n:/bin:/usr/games:/usr/local/games:/snap/bin:/home/dancel/.dotnet/t\
			ools");
		export_edit(tmp, data);
		if (!data->sep[0] || !data->sep[1])
			exit_minishell("Error:\nmalloc failed", -42, data);
		ft_free_tab(data->sep);
		free(tmp);
	}
	init_shlvl(data);
	init_signal(data);
}

char	**convert_cnvp(t_data *data)
{
	char	**mini_envp;
	t_list	*current;
	int		i;

	i = 0;
	mini_envp = malloc(sizeof(char *) * (ft_lstsize(data->cnvp) + 1));
	if (!mini_envp)
		exit_minishell("Error:\nmalloc failed", -42, data);
	current = data->cnvp;
	while (current)
	{
		mini_envp[i] = ft_strdup(current->content);
		if (!mini_envp[i])
		{
			ft_free_tab(mini_envp);
			exit_minishell("Error:\nmalloc failed", -42, data);
		}
		current = current->next;
		i++;
	}
	mini_envp[i] = NULL;
	return (mini_envp);
}

void	unlink_tmps(t_data *data)
{
	t_list	*first;

	first = data->tmps_start;
	while (data->tmps_start)
	{
		unlink(data->tmps_start->content);
		data->tmps_start = data->tmps_start->next;
	}
	ft_lstclear(&first, free);
}

void	reset_data(t_data *data)
{
	if (data->sep != NULL)
		ft_free_tab(data->sep);
	if (data->arg != NULL)
		ft_free_tab(data->arg);
	if (data->token != NULL)
		ft_lstclear(&data->token, free);
	if (data->tmps_start != NULL)
		unlink_tmps(data);
	data->fd_in = STDIN_FILENO;
	data->fd_out = STDOUT_FILENO;
	data->token = NULL;
	data->tmps = NULL;
	data->tmps_start = NULL;
	data->arg = NULL;
	data->sep = NULL;
	data->err_flag = 0;
	data->pipe_flag = 0;
	data->cnt_here = 0;
}
