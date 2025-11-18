/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chajeon <chajeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 00:06:30 by chajeon           #+#    #+#             */
/*   Updated: 2025/02/14 14:00:37 by chajeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	end_heredoc(char *line, char *delim, int *fd, t_data *data)
{
	close(*fd);
	data->exitcode = 0;
	*fd = 0;
	if (!line && g_signal != -130)
	{
		unlink(ft_lstlast(data->tmps)->content);
		ft_putstr_fd("\nminishell: warning: here-document ", STDERR_FILENO);
		ft_putstr_fd("delimited by EOF wanted: ", STDERR_FILENO);
		ft_putstr_fd(delim, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	free(line);
	return (0);
}

static void	write_tmp(char *line, int fd, t_data *data)
{
	char	*res;

	res = ft_strjoin(line, "\n");
	if (!res)
		exit_minishell("Error:\nmalloc failed", -42, data);
	write(fd, res, ft_strlen(res));
	free(line);
	free(res);
	res = NULL;
	line = NULL;
}

static void	check_and_set_line(char **line, t_data *data, int fd)
{
	char	*res;

	res = NULL;
	if (ft_strlen(*line) == 0)
	{
		free(*line);
		*line = ft_strdup("\n");
		if (!*line)
			exit_minishell("Error:\nmalloc failed", -42, data);
	}
	if (ft_strchr(*line, '$'))
	{
		res = key_to_val(data, *line);
		free(*line);
		*line = NULL;
		if (!res)
			exit_minishell("Error:\nmalloc failed", -42, data);
		*line = ft_strdup(res);
		free(res);
		if (!*line)
			exit_minishell("Error:\nmalloc failed", -42, data);
	}
	write_tmp(*line, fd, data);
}

static void	create_filename(t_data *data, int *fd)
{
	char	*front;
	char	*num;
	char	*suffix;
	char	*filename;

	front = "/tmp/heredoc_";
	num = ft_itoa(ft_lstsize(data->tmps));
	if (!num)
		exit_minishell("Error:\nopen failed", -42, data);
	suffix = ".tmp";
	filename = ft_sssjoin(front, num, suffix);
	if (!filename)
	{
		free(num);
		exit_minishell("Error:\nopen failed", -42, data);
	}
	tmp_node(data, filename);
	free(num);
	*fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!*fd)
	{
		free(filename);
		exit_minishell("Error:\nopen failed", -42, data);
	}
	free(filename);
}

void	get_heredoc(char *delim, t_data *data, int *fd, int status)
{
	char	*here;
	int		pid;

	create_filename(data, fd);
	pid = fork();
	if (pid == -1)
		return (error_message("heredoc", errno, data));
	while (pid == 0)
	{
		g_signal = -3;
		here = readline("> ");
		if (!here || !ft_strcmp(here, delim) || g_signal == -130)
			exit(end_heredoc(here, delim, fd, data));
		check_and_set_line(&here, data, *fd);
	}
	if (pid > 0)
	{
		g_signal = -4;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exitcode = WEXITSTATUS(status);
	}
	update_exitcode(data, 1);
	if (data->exitcode == 1)
		data->err_flag = 1;
}
