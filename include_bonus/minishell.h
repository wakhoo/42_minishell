/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:30:59 by dancel            #+#    #+#             */
/*   Updated: 2025/02/20 17:35:28 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdarg.h>
# include <sys/stat.h>
# include <dirent.h>
# include <stdlib.h>
# include <termios.h>
# include <termcap.h>
# include <string.h>
# include <stdio.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <stdbool.h>
# include "libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
# define PREFIX 1
# define SUFFIX 2
# define ALL 3
# define WORD 4
# define INSIDE 5

extern volatile sig_atomic_t	g_signal;

typedef struct s_data
{
	t_list		*cnvp;
	t_list		*token;
	t_list		*tmps;
	t_list		*tmps_start;
	char		**ori;
	char		**arg;
	char		**sep;
	int			cnt_here;
	int			fd_in;
	int			fd_out;
	int			pipe_flag;
	int			err_flag;
	long long	exitcode;
	char		prompt[BUFFER_SIZE];
	int			pipe_flag_2;
}	t_data;

typedef struct s_pipe
{
	int		pipefd[2];
	pid_t	pid;
	pid_t	pid_sec;
	int		save[2];
	int		status;
	int		exitcode;
}	t_pipe;

/*
################################
#    C O R E
################################

# main.c
# ============================*/

void	error_message(char *s, int error, t_data *data);
void	print_tty_prompt(t_data *data);
int		main(int argc, char **argv, char **envp);
/*

# execute.c
# ============================*/
int		execute(char **args, t_data *data, int status);
int		execute_total(t_data *data, int start, int end);
void	select_execute(char **args, t_data *data);
int		execute_sub(t_data *data, int start, int end, int check);
/*

# launch_execute.c
# ============================*/
int		launch_execute_re(t_data *data, int start, int end);
int		launch_execute(t_data *data, int start, int end);
int		launch_router(t_data *data, int start, int end, int index);
int		execute_subre(t_data *data, int start, int end, int reend);
/*

# execute_utils.c
# ============================*/
int		lowest_op(char **tab, int start, int end);
int		find_pipe(char **tab, int start, int end);
int		check_redirec(char **tab, int start, int end, int p_lv);
int		right_res(t_data *data, int end, int index, int left_res);
/*

# pipe.c
# ============================*/
int		execute_pipe(int left_start, int pipe_pos, int right_end, t_data *data);
/*

# signal.c
# ============================*/
void	update_exitcode(t_data *data, int here);
void	sig_handler(int sig, siginfo_t *info, void *context);
void	init_signal(t_data *data);
/*

# wildcards.c
# ============================*/
void	wildcards(t_data *data);
/*

# wildcards_token.c
# ============================*/
void	connect_list(t_data *data, t_list **list, char *new, int check);
/*

################################
#    P A R S I N G
################################

# before_execute.c
# ============================*/
void	rmtokens(t_data *data);
bool	heredoc(t_data *data, t_list *here);
void	tokenize(char *line, t_data *data);
/*

# token_utils.c
# ============================*/
void	rmtokens(t_data *data);
void	token_node(t_data *data, char *s);
void	tmp_node(t_data *data, char *s);
void	tab_to_list(t_data *data, int start, int end);
/*

# redirection.c
# ============================*/
void	get_fd(char *filename, char *symbol, int checker, int *fd);
int		redirection(t_data *data, t_list *cont);
int		current_tmp(t_data *data);
bool	checkfds(t_data *data);
/*

# redirection_utils.c
# ============================*/
void	update_fd(int *fd, int filenum, int checker);
int		checksymbol(char *str);
bool	hasredirection(char **tab, int start, int end);
void	tab_to_list_re(t_data *data, int start, int end, int reend);
/*

# heredoc.c
# ============================*/
void	get_heredoc(char *delim, t_data *data, int *fd, int status);
/*

# err_msg.c
# ============================*/
int		check_access(char *filename);
int		error_check(t_list *cur, t_data *data);
void	heredoc_err(char *str);
/*

# join.c
# ============================*/
char	*ft_strcjoin(char *str, int c);
char	*ft_sssjoin(char *str1, char *str2, char *str3);
/*

# init_data.c
# ============================*/
void	unlink_tmps(t_data *data);
void	init_data(char **envp, t_data *data);
char	**convert_cnvp(t_data *data);
void	reset_data(t_data *data);
/*

# replace_env.c
# ============================*/
char	*get_value(char *str, int i, int keylen, t_data *data);
void	replace(t_data *data, char **tmp, int *i);
void	repexitcode(t_data *data, char **tmp, int *i);
char	*key_to_val(t_data *data, char *str);
void	change_env(t_data *data);
/*

# has_util.c
# ============================*/
int		haveenv(char *str);
bool	hasquotes(char *str);
/*

# checker_util.c
# ============================*/
int		close_index(char **tab, int start, int end);
bool	checkdilm(char *str);
int		check_meta(char *str, int i);
int		check_quotes_and_space(char *line);
/*

# convert.c
# ============================*/
void	list_to_array(t_data *data, char ***tab);
void	char_to_list(char *line, t_data *data, int start);
void	convert_envp(char *s, t_data *data);
/*

# count.c
# ============================*/
int		cnt_n_keyname(char *str, int start);
int		n_quotes(char *str, int i);
int		cnt_word(char *str, int start);
int		pass_space(char *str, int i);
int		p_close(char *str);
/*

################################
#    B U I L T I N S
################################

# cd.c
# ============================*/
void	cd(t_data *data);
/*

# echo.c
# ============================*/
void	echo(t_data *data);
/*

# env.c
# ============================*/
char	*ft_getenv(char *var, t_data *data);
void	export_edit(char *s, t_data *data);
void	env(t_data *data);
/*

# exit.c
# ============================*/
void	exit_minishell(char *error_msg, int exitcode, t_data *data);
/*

# export.c
# ============================*/
void	print_export(t_data *data);
void	export_var(char *var, char *value, char *arg, t_data *data);
void	export(int i, t_data *data);
/*

# pwd.c
# ============================*/
void	pwd(t_data *data);
/*

# unset.c
# ============================*/
void	unset(t_data *data);

#endif