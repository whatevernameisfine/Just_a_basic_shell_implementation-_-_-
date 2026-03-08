/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:20:00 by riramana          #+#    #+#             */
/*   Updated: 2025/11/14 10:44:01 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_redir_char(char c)
{
	return (c == '<' || c == '>');
}

static void	init_pipe_validation(int *i, int *lp, int *sq, int *dq)
{
	*i = 0;
	*lp = 0;
	*sq = 0;
	*dq = 0;
}

static int	check_pipe_char(char c, int *lp, int sq, int dq)
{
	if (c == '|' && !sq && !dq)
	{
		if (*lp)
			return (0);
		*lp = 1;
	}
	else if (c != ' ' && c != '\t')
		*lp = 0;
	return (1);
}

int	validate_pipe_syntax(char *command)
{
	int	i;
	int	last_was_pipe;
	int	in_single_quote;
	int	in_double_quote;

	init_pipe_validation(&i, &last_was_pipe, &in_single_quote,
		&in_double_quote);
	while (command[i] && (command[i] == ' ' || command[i] == '\t'))
		i++;
	if (command[i] == '|')
		return (0);
	while (command[i])
	{
		if (command[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (command[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (!check_pipe_char(command[i], &last_was_pipe, in_single_quote,
				in_double_quote))
			return (0);
		i++;
	}
	if (last_was_pipe)
		return (0);
	return (1);
}

int	has_redir_before_pipe(char *cmd, int sq, int dq)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' && !dq)
			sq = !sq;
		else if (cmd[i] == '"' && !sq)
			dq = !dq;
		else if (!sq && !dq && is_redir_char(cmd[i]))
		{
			while (cmd[i] && is_redir_char(cmd[i]))
				i++;
			while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
				i++;
			if (cmd[i] == '|')
				return (1);
			continue ;
		}
		if (cmd[i])
			i++;
	}
	return (0);
}
