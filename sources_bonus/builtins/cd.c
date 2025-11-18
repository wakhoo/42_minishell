/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:21:27 by dancel            #+#    #+#             */
/*   Updated: 2025/02/06 16:36:34 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_home(t_data *data)
{
	if (!data->sep[1])
	{
		ft_free_tab(data->sep);
		data->sep = malloc(sizeof(char *) * (3));
		data->sep[0] = ft_strdup("cd");
		data->sep[2] = NULL;
	}
	else
		free(data->sep[1]);
}

static int	cd_special_arg(t_data *data)
{
	if (!data->sep[1] || (data->sep[1][0] == '~' && !data->sep[1][1]))
	{
		if (!ft_getenv("HOME=", data))
			return (data->exitcode = 1, \
			printf("minishell: cd: HOME not set\n"), 0);
		cd_home(data);
		data->sep[1] = ft_strdup(ft_getenv("HOME=", data));
	}
	else if (data->sep[1] && data->sep[1][0] == '-' && !data->sep[1][1])
	{
		if (!ft_getenv("OLDPWD=", data))
			return (data->exitcode = 1, \
			printf("minishell: cd: OLDPWD not set\n"), 0);
		free(data->sep[1]);
		data->sep[1] = ft_strdup(ft_getenv("OLDPWD=", data));
	}
	else if (data->sep[2])
		return (data->exitcode = 1, \
		printf("minishell: cd: too many arguments\n"), 0);
	if (!data->sep[1])
		exit_minishell("Error:\nmalloc failed", -42, data);
	return (1);
}

void	cd(t_data *data)
{
	char		oldpwd[BUFFER_SIZE];
	char		pwd[BUFFER_SIZE];

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		exit_minishell("Error:\ngetcwd failed", -42, data);
	if (!cd_special_arg(data))
		return ;
	if (data->sep[1] && chdir(data->sep[1]) == -1)
		return (error_message(data->sep[0], errno, data));
	if (!getcwd(pwd, sizeof(pwd)))
		exit_minishell("Error:\ngetcwd failed", -42, data);
	ft_free_tab(data->sep);
	data->sep = malloc(sizeof(char *) * (4));
	data->sep[0] = ft_strdup("export");
	data->sep[1] = ft_strjoin("OLDPWD=", oldpwd);
	data->sep[2] = ft_strjoin("PWD=", pwd);
	data->sep[3] = NULL;
	if (!data->sep[0] || !data->sep[1] || !data->sep[2])
		exit_minishell("Error:\nmalloc failed", -42, data);
	export(0, data);
	data->exitcode = 0;
}
