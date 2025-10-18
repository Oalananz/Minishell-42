/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_open.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:40:04 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/30 18:21:26 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredoc(t_token *tokens)
{
	t_token	*temp;
	int		counter;
	int		i;

	temp = tokens;
	counter = 0;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == HEREDOC)
			counter++;
		i++;
	}
	return (counter);
}

void	heredoc_ctrl_d(char *text, char *exit_heredoc
		, t_shell *shell, t_fds *fd)
{
	char	*tmp;
	char	*t;

	tmp = ft_strjoin("ARSSH: here-document delimited by end-of-file (wanted `",
			exit_heredoc);
	t = ft_strjoin(tmp, "\')\n");
	free(tmp);
	write(2, t, ft_strlen(t));
	free(t);
	if (text)
	{
		free(text);
		text = NULL;
	}
	if (exit_heredoc)
	{
		free(exit_heredoc);
		exit_heredoc = NULL;
	}
	free_heredoc(shell, fd);
	exit(0);
}

void	ctrl_c_multiple(char *file, int fd, t_shell *shell, char *exit_heredoc)
{
	if (fd > 0)
	{
		close (fd);
		fd = -1;
	}
	if (exit_heredoc)
	{
		free(exit_heredoc);
		exit_heredoc = NULL;
	}
	if (!access(file, F_OK))
		unlink(file);
	g_signal = 0;
	dup2(shell->fd_in, STDIN_FILENO);
	close(shell->fd_in);
	free_shell(shell);
	exit(128 + SIGINT);
}

int	open_heredocs(t_shell *shell, char *exit_heredoc, char *file)
{
	char	*text;

	shell->fd_in = dup(STDIN_FILENO);
	if (exit_heredoc[0] == '\'' || exit_heredoc[0] == '\"')
		exit_heredoc = remove_qoutes(exit_heredoc, shell);
	shell->fd_heredoc = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	text = NULL;
	rl_loop(shell, text, exit_heredoc, file);
	return (shell->fd_heredoc);
}

int	create_heredoc_files(t_token *tokens)
{
	t_token	*temp;
	int		i;
	int		count;
	char	*str;

	temp = tokens;
	count = 1;
	while (temp)
	{
		i = 0;
		while (temp->content[i])
		{
			if (temp->type[i] == HEREDOC)
			{
				str = ft_itoa(count);
				temp->heredoc_file = ft_strjoin(".temp", str);
				free (str);
				count++;
				break ;
			}
			i++;
		}
		temp = temp->next;
	}
	return (count);
}
