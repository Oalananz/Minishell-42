/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:04:23 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/30 15:08:21 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirections(char **lst, int *i)
{
	if (!ft_strcmp(lst[(*i)], ">") || !ft_strcmp(lst[(*i)], "<")
		|| !ft_strcmp(lst[(*i)], ">>") || !ft_strcmp(lst[(*i)], "<<"))
		return (0);
	return (1);
}

int	redirect_in(t_token *tokens)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = tokens;
	while (tmp->content[i])
	{
		if (tmp->type[i] == REDIRECTIN || tmp->type[i] == HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

void	check_last_redirect_in(t_fds *fd, t_token *tokens)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = tokens;
	while (tmp->content[i])
	{
		if (tmp->type[i] == HEREDOC)
			fd->flag_heredoc = 1;
		else if (tmp->type[i] == REDIRECTIN)
			fd->flag_heredoc = 0;
		i++;
	}
}

int	is_there_arg(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == ARGUMENT)
			return (1);
		i++;
	}
	return (0);
}

int	is_there_text(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == TEXT)
			return (1);
		i++;
	}
	return (0);
}
