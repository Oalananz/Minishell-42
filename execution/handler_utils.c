/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:58:28 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/30 14:58:59 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_execute_multiple(t_shell *shell)
{
	if (shell)
	{
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
			free (shell->exe);
			shell->exe = NULL;
		}
	}
}

int	is_there_heredocs(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp)
	{
		i = 0;
		while (temp->content[i])
		{
			if (temp->type[i] == HEREDOC)
				return (1);
			i++;
		}
		temp = temp->next;
	}
	return (0);
}
