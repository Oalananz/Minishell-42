/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:05:04 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/30 18:29:12 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child(t_fds *fds, t_shell *shell)
{
	fds->saved_in = dup(STDIN_FILENO);
	set_signals_heredoc();
	if (fds->delimiter && (fds->delimiter[0] == '\''
			|| fds->delimiter[0] == '\"'))
		fds->delimiter = remove_qoutes(fds->delimiter, shell);
	while (1)
	{
		fds->text = readline("> ");
		ctrl_c(shell, fds);
		if (!fds->text)
		{
			close (fds->saved_in);
			close(fds->fd_in[fds->index_j]);
			heredoc_ctrl_d(fds->text, fds->delimiter, shell, fds);
		}
		if (!shell->expand_flag)
			fds->text = expand_heredoc(fds->text, shell);
		if (!ft_strcmp(fds->text, fds->delimiter))
			exit_delimiter(shell, fds);
		write(fds->fd_in[fds->index_j], fds->text, ft_strlen(fds->text));
		write(fds->fd_in[fds->index_j], "\n", 1);
		free(fds->text);
	}
	free(fds->delimiter);
}

void	heredoc_parent(pid_t pid, t_shell *shell, struct sigaction original_sa)
{
	int					status;
	struct sigaction	sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigemptyset(&sa_ignore.sa_mask);
	sigaction(SIGINT, &sa_ignore, NULL);
	waitpid(pid, &status, 0);
	sigaction(SIGINT, &original_sa, NULL);
	if (WIFEXITED(status))
	{
		shell->exit_status = WEXITSTATUS(status);
		if (shell->exit_status == 128 + SIGINT)
			shell->heredoc_interrupted = 1;
	}
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			shell->heredoc_interrupted = 1;
	}
}

int	check_ctrl_c_flag(t_shell *shell, t_fds *fds)
{
	if (shell->heredoc_interrupted)
	{
		if (fds->fd_in[0] > 0)
			close(fds->fd_in[0]);
		if (fds->temp)
			unlink(fds->temp);
		return (shell->heredoc_interrupted);
	}
	return (shell->heredoc_interrupted);
}

static void	handle_heredoc_loop(t_shell *shell, char **lst, t_fds *fds,
		struct sigaction original_sa)
{
	while (lst[fds->index_i])
	{
		if (!ft_strcmp(lst[fds->index_i], "<<"))
		{
			fds->index_i++;
			open_file(fds, fds->index_j);
			fds->pid = fork();
			if (fds->pid == 0)
			{
				fds->delimiter = ft_strdup(lst[fds->index_i]);
				if (!fds->delimiter)
					exit(1);
				handle_heredoc_child();
				child(fds, shell);
				close(fds->fd_in[0]);
			}
			else if (fds->pid > 0)
				heredoc_parent(fds->pid, shell, original_sa);
			close(fds->fd_in[0]);
			if (check_ctrl_c_flag(shell, fds))
				break ;
			fds->index_j++;
		}
		fds->index_i++;
	}
}

void	open_heredoc(t_shell *shell, char **lst, t_fds *fds)
{
	struct sigaction	original_sa;

	fds->temp = ft_strdup(".temp");
	fds->flag_expand = 1;
	fds->index_i = 0;
	fds->index_j = 0;
	shell->heredoc_interrupted = 0;
	sigaction(SIGINT, NULL, &original_sa);
	handle_heredoc_loop(shell, lst, fds, original_sa);
	fds->flag_heredoc = 1;
	sigaction(SIGINT, &original_sa, NULL);
}
