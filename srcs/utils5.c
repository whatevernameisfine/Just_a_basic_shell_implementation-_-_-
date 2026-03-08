/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:54:05 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:50:18 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_heredoc_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
	ft_putstr_fd("end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

int	is_input_redir_operator(char *str, int pos, int in_sq, int in_dq)
{
	if (in_sq || in_dq)
		return (0);
	if (str[pos] == '<' && str[pos + 1] == '<')
		return (2);
	if (str[pos] == '<')
		return (1);
	return (0);
}

char	**split_echo_args_with_data(char *command, t_shell_data *data)
{
	char	**args;
	int		arg_count;

	arg_count = count_echo_args(command);
	args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	if (!parse_args_with_data(command, args, data))
		return (NULL);
	return (args);
}

int	is_redir_operator(char *str, int pos, int in_sq, int in_dq)
{
	if (in_sq || in_dq)
		return (0);
	if (str[pos] == '<' && str[pos + 1] == '<')
		return (0);
	if (str[pos] == '>' && str[pos + 1] == '>')
		return (2);
	if (str[pos] == '>')
		return (1);
	return (0);
}

void	skip_quotes(char *str, int *pos, int *in_sq, int *in_dq)
{
	if (str[*pos] == '\'' && !(*in_dq))
		*in_sq = !(*in_sq);
	else if (str[*pos] == '"' && !(*in_sq))
		*in_dq = !(*in_dq);
}
