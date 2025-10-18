/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 02:10:24 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/30 16:51:27 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_exit_status(int id, t_shell *shell)
{
	int	status;

	status = 0;
	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	close_pipes(int **pipe, t_shell *shell)
{
	int	i;

	i = 0;
	if (!pipe)
		return ;
	while (i < shell->exe->pipes_count)
	{
		if (pipe[i][0] && pipe[i][0] > 0)
			close(pipe[i][0]);
		if (pipe[i][1] && pipe[i][1] > 0)
			close(pipe[i][1]);
		i++;
	}
}

int	valid_arg(char *status)
{
	int	i;

	i = 0;
	while (status[i])
	{
		if (ft_isdigit(status[i]))
			i++;
		else
			return (-1);
	}
	return (1);
}
