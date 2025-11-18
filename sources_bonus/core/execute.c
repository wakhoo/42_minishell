/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:08:45 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/20 18:21:19 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	find_exec(char **args, int i, t_data *data)
{
	char	**path;
	char	try[BUFFER_SIZE];
	int		n;

	path = ft_split(ft_getenv("PATH=", data), ':');
	if (!path)
		return ;
	while (path[++i])
	{
		n = ft_strlen(path[i]);
		ft_memcpy(try, path[i], n);
		try[n] = '/';
		ft_memcpy(try + n + 1, args[0], ft_strlen(args[0]));
		try[n + 1 + ft_strlen(args[0])] = '\0';
		if (access(try, F_OK | X_OK) == 0)
		{
			free(args[0]);
			args[0] = ft_strdup(try);
			break ;
		}
	}
	ft_free_tab(path);
	if (!args[0])
		exit_minishell("Error:\nmalloc failed\n", -42, data);
}

int	execute(char **args, t_data *data, int status)
{
	char	**env;

	if (args[0][0] != '.' && !ft_strchr(args[0], '/'))
		find_exec(args, -1, data);
	env = convert_cnvp(data);
	g_signal = fork();
	if (g_signal == -1)
		return (error_message(args[0], errno, data), -1);
	if (!(ft_strsuffix(args[0], "/minishell")))
		init_signal(data);
	if (g_signal == 0)
	{
		if (data->pipe_flag == 0)
			g_signal = ft_strsuffix(args[0], "/minishell");
		if (execve(args[0], args, env) == -1)
		{
			error_message(args[0], errno, data);
			exit(127);
		}
	}
	else if (data->pipe_flag == 0)
		g_signal = !(ft_strsuffix(args[0], "/minishell"));
	return (wait(&status), ft_free_tab(env), WEXITSTATUS(status));
}

void	select_execute(char **args, t_data *data)
{
	if (!ft_strcmp(args[0], "cd"))
		cd(data);
	else if (!ft_strcmp(args[0], "export"))
		export(0, data);
	else if (!ft_strcmp(args[0], "unset"))
		unset(data);
	else if (!ft_strcmp(args[0], "env"))
		env(data);
	else if (!ft_strcmp(args[0], "echo"))
		echo(data);
	else if (!ft_strcmp(args[0], "exit"))
		exit_minishell("", 0, data);
	else if (!ft_strcmp(args[0], "pwd"))
		pwd(data);
	else
		data->exitcode = execute(args, data, 0);
}

int	execute_sub(t_data *data, int start, int end, int check)
{
	pid_t	pid;
	int		status;
	int		value;

	pid = fork();
	if (pid == -1)
		return (error_message(data->arg[start], errno, data), -1);
	else if (pid == 0)
	{
		value = execute_total(data, start, end);
		exit(value);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (check != -1)
		{
			dup2(check, STDOUT_FILENO);
			close(check);
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (-1);
	}
}

int	execute_total(t_data *data, int start, int end)
{
	int	index;
	int	left_res;
	int	redirec;

	if (start > end)
		return (-1);
	redirec = check_redirec(data->arg, start, end, 0);
	index = close_index(data->arg, start, end);
	if (index != -1 && (index == end || end == redirec -1)
		&& !ft_strcmp(data->arg[start], "(")
		&& !ft_strcmp(data->arg[index], ")"))
		return (execute_subre(data, start + 1, index - 1, redirec));
	index = lowest_op(data->arg, start, end);
	if (index != -1)
	{
		left_res = execute_total(data, start, index - 1);
		return (right_res(data, end, index, left_res));
	}
	index = find_pipe(data->arg, start, end);
	if (index != -1)
		return (execute_pipe(start, index, end, data));
	data->exitcode = launch_router(data, start, end, index);
	return ((int)data->exitcode);
}
