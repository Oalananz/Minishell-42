/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:10:25 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/30 18:28:40 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc(t_shell *shell, t_fds *fd)
{
	free_shell(shell);
	shell = NULL;
	if (fd && fd->temp)
		free(fd->temp);
	if (fd)
		free(fd);
}

void	set_signals_heredoc(void)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ctrl_c(t_shell *shell, t_fds *fd)
{
	if (g_signal == SIGINT)
	{
		close (fd->fd_in[0]);
		free(fd->delimiter);
		g_signal = 0;
		dup2(fd->saved_in, STDIN_FILENO);
		close (fd->saved_in);
		free_heredoc(shell, fd);
		exit(128 + SIGINT);
	}
}

void	exit_delimiter(t_shell *shell, t_fds *fd)
{
	if (fd->text)
	{
		free(fd->text);
		fd->text = NULL;
	}
	if (fd->delimiter)
	{
		free(fd->delimiter);
		fd->delimiter = NULL;
	}
	if (fd->fd_in[0] > 0)
	{
		close(fd->fd_in[0]);
		fd->fd_in[0] = -1;
	}
	if (fd->saved_in > 0)
	{
		close(fd->saved_in);
		fd->saved_in = -1;
	}
	if (shell && fd)
		free_heredoc(shell, fd);
	exit(0);
}
