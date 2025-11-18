/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   pipe.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dancel <dancel@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/12 16:42:31 by dancel			#+#	#+#			 */
/*   Updated: 2025/02/12 19:01:13 by dancel		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

int	find_pipe(char **tab, int start, int end)
{
	int	p_lv;

	p_lv = 0;
	while (tab[start] && start <= end)
	{
		if (!ft_strcmp(tab[start], "("))
			p_lv += 1;
		else if (!ft_strcmp(tab[start], ")"))
			p_lv -= 1;
		else if (!ft_strcmp(tab[start], "|") && p_lv == 0)
			return (start);
		start ++;
	}
	return (-1);
}

static void	cnt_usedheredoc(int start, int end, t_data *data)
{
	t_list	*curr;

	tab_to_list(data, start, end);
	curr = data->token;
	while (curr != NULL)
	{
		if (checksymbol(curr->content) > 0
			&& checksymbol(curr->content) < 4)
		{
			if (!ft_strcmp(curr->content, "<<") && !data->err_flag)
				data->cnt_here++;
		}
		curr = curr->next;
	}
}

static void	init_pipe(int *save, int *pipefd, t_data *data)
{
	save[0] = dup(0);
	save[1] = dup(1);
	if (pipe(pipefd) == -1)
		error_message("pipe", errno, data);
	init_signal(data);
	g_signal = -5;
}

static void	endpipe(t_data *data, t_pipe arg)
{
	close(arg.pipefd[0]);
	while (wait(&arg.status) > 0)
	{
		if (WIFEXITED(arg.status))
		{
			arg.exitcode = WEXITSTATUS(arg.status);
			if (arg.exitcode > data->exitcode)
				data->exitcode = arg.exitcode;
		}
	}
	ft_dup2(arg.save[0], 0);
	ft_dup2(arg.save[1], 1);
	close(arg.save[0]);
	close(arg.save[1]);
}

int	execute_pipe(int left_start, int pipe_pos, int right_end, t_data *data)
{
	t_pipe	arg;

	data->pipe_flag = 1;
	init_pipe(arg.save, arg.pipefd, data);
	cnt_usedheredoc(left_start, pipe_pos - 1, data);
	arg.pid = fork();
	if (arg.pid == -1)
		return (error_message("fork", errno, data), 1);
	if (arg.pid == 0)
	{
		close(arg.pipefd[0]);
		ft_dup2(arg.pipefd[1], 1);
		exit(execute_total(data, left_start, pipe_pos - 1));
	}
	close(arg.pipefd[1]);
	arg.pid_sec = fork();
	if (arg.pid_sec == -1)
		return (error_message("fork", errno, data), 1);
	if (arg.pid_sec == 0)
	{
		ft_dup2(arg.pipefd[0], 0);
		exit(execute_total(data, pipe_pos + 1, right_end));
	}
	return (endpipe(data, arg), data->exitcode);
}
