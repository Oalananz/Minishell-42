/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:47:51 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/29 20:14:27 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_d(t_shell *shell)
{
	int	x;

	printf("exit\n");
	if (shell->env)
		free_env(shell->env);
	x = shell->exit_status;
	if (shell)
	{
		free(shell);
		shell = NULL;
	}
	exit(x);
}

void	unlink_files(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->heredoc_file)
		{
			if (!access(tmp->heredoc_file, F_OK))
				unlink(tmp->heredoc_file);
		}
		tmp = tmp->next;
	}
}

void	free_loop(t_shell *shell, t_token *tokens)
{
	if (tokens)
		execute(shell, tokens);
	if (shell && shell->prompt)
	{
		unlink_files(shell->head);
		if (shell->exe)
		{
			if (shell->exe->pipes)
			{
				ft_free_int2d(shell->exe->pipes, shell);
				shell->exe->pipes = NULL;
			}
			if (shell->exe->pids)
			{
				free(shell->exe->pids);
				shell->exe->pids = NULL;
			}
			free(shell->exe);
			shell->exe = NULL;
		}
		free(shell->prompt);
		shell->prompt = NULL;
	}
	if (tokens)
		free_tokenizer(tokens);
}
