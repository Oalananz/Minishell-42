/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:47:32 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/30 13:56:53 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_int2d(int **arr, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->exe->pipes_count && arr)
	{
		if (arr && arr[i])
		{
			free(arr[i]);
		}
		i++;
	}
	free(arr);
	arr = NULL;
}

void	exit_execution_helper(t_shell *shell)
{
	if (shell->enviroment)
	{
		ft_free_2d(shell->enviroment);
		shell->enviroment = NULL;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->cmd_list)
	{
		ft_free_2d(shell->cmd_list);
		shell->cmd_list = NULL;
	}
	if (shell->paths)
	{
		ft_free_2d(shell->paths);
		shell->paths = NULL;
	}
}

void	exit_execution(t_shell *shell, t_token *tokens)
{
	if (tokens && shell && shell->head)
	{
		free_tokenizer(shell->head);
		tokens = NULL;
	}
	if (shell)
	{
		if (shell->exe)
		{
			if (shell->exe->cmd)
			{
				free(shell->exe->cmd);
				shell->exe->cmd = NULL;
			}
			free_exe(shell->exe, shell);
		}
		exit_execution_helper(shell);
		free(shell);
		shell = NULL;
	}
}

void	exit_execute(t_shell *shell, t_token *tokens)
{
	if (tokens)
		free_tokenizer(tokens);
	if (shell->enviroment)
		ft_free_2d(shell->enviroment);
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd_list)
		ft_free_2d(shell->cmd_list);
	if (shell)
		free(shell);
}

void	cmd_not_found(t_shell *shell)
{
	char	*temp;
	char	*string;

	temp = ft_strjoin("command not found: ", shell->cmd_list[0]);
	string = ft_strjoin(temp, "\n");
	write(2, string, ft_strlen(string));
	free(temp);
	free(string);
}
