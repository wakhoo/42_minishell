/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:45:14 by dancel            #+#    #+#             */
/*   Updated: 2025/02/20 18:26:44 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_exitcode(t_data *data, int here)
{
	signal(SIGQUIT, SIG_IGN);
	if (g_signal == -130 && here)
		data->err_flag = 1;
	if (g_signal < -5 || g_signal == -1)
		data->exitcode = -g_signal;
	g_signal = -2;
}

static void	sig_handler_2(int sig)
{
	if (sig == SIGINT && g_signal == -3)
		exit(130);
	if (sig == SIGINT && (g_signal == -4))
	{
		if (g_signal == -4)
			printf("^C");
		printf("\n");
		g_signal = -130;
	}
	if (sig == SIGQUIT && (g_signal > 0))
	{
		printf("Quit\n");
		g_signal = -131;
	}
}

void	sig_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (g_signal == -5)
	{
		if (sig == SIGINT)
			g_signal = -130;
		if (sig == SIGQUIT)
			g_signal = -131;
		return ;
	}
	if (sig == SIGINT && (g_signal == -2 || g_signal > 0 || g_signal == -130))
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (g_signal == -2 || g_signal == -130)
			rl_redisplay();
		g_signal = -130;
	}
	sig_handler_2(sig);
}

void	init_signal(t_data *data)
{
	struct sigaction	sa;
	sigset_t			sigmask;

	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGINT);
	sigaddset(&sigmask, SIGQUIT);
	sa.sa_sigaction = sig_handler;
	sa.sa_flags = SA_RESTART;
	sa.sa_mask = sigmask;
	if (sigaction(SIGINT, &sa, NULL) == -1
		|| sigaction(SIGQUIT, &sa, NULL) == -1)
		return (error_message("signal", errno, data));
}
