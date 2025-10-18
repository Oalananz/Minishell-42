/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:55:12 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/30 14:56:04 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->variable)
			free(env->variable);
		if (env->content)
			free(env->content);
		free(env);
		env = tmp;
	}
	free(env);
}

void	free_tokenizer(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->content)
			ft_free_2d(tokens->content);
		if (tokens->type)
			free(tokens->type);
		if (tokens->heredoc_file)
		{
			free(tokens->heredoc_file);
		}
		free(tokens);
		tokens = temp;
	}
	free(tokens);
	tokens = NULL;
}

void	free_exe(t_execute *exe, t_shell *shell)
{
	if (exe->pids)
	{
		free(exe->pids);
		exe->pids = NULL;
	}
	if (exe->pipes)
	{
		close_pipes(exe->pipes, shell);
		ft_free_int2d(exe->pipes, shell);
		exe->pipes = NULL;
	}
	free(exe);
}

void	free_shell(t_shell *shell)
{
	if (shell)
	{
		if (shell->head)
			free_tokenizer(shell->head);
		if (shell->cmd_list)
			ft_free_2d(shell->cmd_list);
		if (shell->paths)
			ft_free_2d(shell->paths);
		if (shell->prompt)
			free(shell->prompt);
		if (shell->enviroment)
			ft_free_2d(shell->enviroment);
		if (shell->variable)
			free(shell->variable);
		if (shell->env)
			free_env(shell->env);
		if (shell->exe)
			free_exe(shell->exe, shell);
		free(shell);
	}
}
