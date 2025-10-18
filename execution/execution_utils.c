/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:28:24 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/30 15:07:11 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	how_many_pipes(t_token *tokens)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = tokens;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i - 1);
}

int	redirect_first_arg(t_token *tokens)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = tokens;
	if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
		|| temp->type[i] == APPEND || temp->type[i] == HEREDOC)
		return (1);
	return (0);
}

int	is_there_redirect(t_token *tokens)
{
	t_token	*temp;
	int		i;
	int		redirect;

	temp = tokens;
	i = 0;
	redirect = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
			|| temp->type[i] == HEREDOC || temp->type[i] == APPEND)
		{
			i += 2;
			redirect++;
		}
		else
			i++;
	}
	return (redirect);
}

int	is_there_command(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == COMMAND)
			return (1);
		i++;
	}
	return (0);
}

// int	is_there_arg(t_token *tokens)
// {
// 	t_token	*temp;
// 	int		i;

// 	temp = tokens;
// 	i = 0;
// 	while (temp->content[i])
// 	{
// 		if (temp->type[i] == ARGUMENT)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// int	is_there_text(t_token *tokens)
// {
// 	t_token	*temp;
// 	int		i;

// 	temp = tokens;
// 	i = 0;
// 	while (temp->content[i])
// 	{
// 		if (temp->type[i] == TEXT)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

int	is_there_heredoc(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == HEREDOC)
			return (1);
		i++;
	}
	return (0);
}
