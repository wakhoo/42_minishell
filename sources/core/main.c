/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:42:56 by dancel            #+#    #+#             */
/*   Updated: 2025/02/20 18:27:25 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	error_message(char *s, int error, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(s, 2);
	if (error == ENOENT)
		ft_putstr_fd(": No such file or directory\n", 2);
	if (error == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	if (error == ENOTDIR)
		ft_putstr_fd(": Not a directory\n", 2);
	if (error == ELOOP)
		ft_putstr_fd(": Too many symbolic links encountered\n", 2);
	if (error == ELIBBAD)
		ft_putstr_fd(": Exec format error. Wrong Architecture\n", 2);
	if (error == ENOMEM)
		ft_putstr_fd(": Cannot allocate memory\n", 2);
	if (error == EPERM)
		ft_putstr_fd(": Operation not permitted\n", 2);
	if (error == EAGAIN)
		ft_putstr_fd(": Resource temporarily unavailable\n", 2);
	if (error == EFAULT)
		ft_putstr_fd(": Bad address\n", 2);
	if (error == EINVAL)
		ft_putstr_fd(": Erreur sigaction: Invalid argument\n", 2);
	if (error == EIO)
		ft_putstr_fd(": Input/output error\n", 2);
	data->exitcode = 1;
}

void	print_tty_prompt(t_data *data)
{
	int		i;

	if (!ft_lstchr(data->cnvp, "_="))
		export_edit("_=/usr/bin/env", data);
	i = ft_strcpy(data->prompt, "\033[1;31m");
	if (ft_getenv("USER=", data))
		i = ft_strcpy(data->prompt + i, ft_getenv("USER=", data)) + i;
	if (ft_getenv("SESSION_MANAGER=", data)
		&& strstr(ft_getenv("SESSION_MANAGER=", data), ".42"))
	{
		data->prompt[i++] = '@';
		i = ft_strcpy(data->prompt + i, \
		ft_getenv("SESSION_MANAGER=", data) + 6) + i;
		i = i - ft_strlen(strstr(ft_getenv("SESSION_MANAGER=", data), ".42"));
	}
	i = ft_strcpy(data->prompt + i, "\033[0m") + i;
	data->prompt[i++] = ':';
	i = ft_strcpy(data->prompt + i, "\033[1;33m") + i;
	if (ft_getenv("PWD=", data))
		i = ft_strcpy(data->prompt + i, ft_getenv("PWD=", data)) + i;
	i = ft_strcpy(data->prompt + i, "$ ") + i;
	i = ft_strcpy(data->prompt + i, "\033[0m") + i;
}

static void	main_loop_2(t_data *data)
{
	execute_total(data, 0, ft_tab_size(data->arg) - 1);
	if (g_signal == -130 && data->pipe_flag)
		printf("\n");
	if (g_signal == -131 && data->pipe_flag)
	{
		printf("Quit\n");
		g_signal = -131;
	}
}

void	main_loop(t_data *data)
{
	char	*line;

	while (1)
	{
		update_exitcode(data, 0);
		print_tty_prompt(data);
		line = readline(data->prompt);
		if (!line)
			break ;
		if (!line[0])
			continue ;
		if (!check_quotes_and_space(line))
		{
			free(line);
			line = NULL;
			continue ;
		}
		add_history(line);
		data->ori = &line;
		tokenize(line, data);
		if (data->err_flag || ft_lstsize(data->token) == 0)
			continue ;
		main_loop_2(data);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	(void)argv;
	if (argc != 1)
		return (write(2, "Error:\n<usage> ./minishell", 26), 0);
	data = malloc(sizeof(t_data));
	if (!data)
		return (write(2, "Error:\nmalloc failed", 20), 0);
	init_data(envp, data);
	main_loop(data);
	exit_minishell("", 0, data);
}
