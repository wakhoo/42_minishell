/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancel <dancel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:21:22 by dancel            #+#    #+#             */
/*   Updated: 2025/01/25 18:22:38 by dancel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_data *data)
{
	char	pwd[BUFFER_SIZE];

	printf("%s\n", getcwd(pwd, BUFFER_SIZE));
	data->exitcode = 0;
}
