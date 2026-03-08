/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:21:55 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 16:16:43 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	close_parent_pipes(int i, int (*pipes)[2])
{
	if (i > 0)
		close(pipes[i - 1][0]);
}

int	validate_pipeline_syntax(char **cmd_strs, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (!cmd_strs[i] || is_whitespace(cmd_strs[i]))
		{
			ft_putstr_fd("minishell: syntax error near ", 2);
			ft_putstr_fd("unexpected token `|'\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

void	signal_thing(int sig)
{
	g_signal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_exec(int sig)
{
	g_signal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_done = 1;
}
