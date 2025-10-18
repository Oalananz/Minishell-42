/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:28:42 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/30 17:33:03 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child(struct sigaction sa, int i, t_shell *shell, t_token *temp)
{
	int		fd;
	char	*exit_heredoc;

	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	fd = 0;
	sa.sa_handler = heredoc_signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	exit_heredoc = ft_strdup(temp->content[i + 1]);
	fd = open_heredocs(shell, exit_heredoc, temp->heredoc_file);
	if (fd)
		close(fd);
	free_shell(shell);
	exit(0);
}

static void	process(t_token *temp, struct sigaction original_sa,
		struct sigaction sa, t_shell *shell)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == HEREDOC)
		{
			pid = fork();
			if (pid == 0)
				child(sa, i, shell, temp);
			else if (pid > 0)
				heredoc_parent(pid, shell, original_sa);
			if (shell->heredoc_interrupted)
				break ;
		}
		i++;
	}
}

void	heredoc_handle(t_token *tokens, t_shell *shell)
{
	t_token				*temp;
	struct sigaction	sa;
	struct sigaction	original_sa;

	shell->heredoc_interrupted = 0;
	temp = tokens;
	sigaction(SIGINT, NULL, &original_sa);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	while (temp)
	{
		process(temp, original_sa, sa, shell);
		if (shell->heredoc_interrupted)
			break ;
		temp = temp->next;
	}
}
