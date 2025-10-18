/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_open_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:15:06 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/30 18:25:45 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_not_text(int fd, t_shell *shell, char *exit_heredoc, char *text)
{
	close (fd);
	close (shell->fd_in);
	heredoc_ctrl_d(text, exit_heredoc, shell, NULL);
}

void	rl_loop(t_shell *shell, char *text, char *exit_heredoc, char *file)
{
	while (1)
	{
		text = readline("> ");
		if (g_signal == SIGINT)
			ctrl_c_multiple(file, shell->fd_heredoc, shell, exit_heredoc);
		if (!text)
			if_not_text(shell->fd_heredoc, shell, exit_heredoc, text);
		if (shell->expand_flag)
			text = expand_heredoc(text, shell);
		if (text && exit_heredoc && !ft_strcmp(text, exit_heredoc))
		{
			close (shell->fd_in);
			free(text);
			if (exit_heredoc)
			{
				free(exit_heredoc);
				exit_heredoc = NULL;
			}
			text = NULL;
			break ;
		}
		write(shell->fd_heredoc, text, ft_strlen(text));
		write(shell->fd_heredoc, "\n", 1);
		free(text);
	}
}
