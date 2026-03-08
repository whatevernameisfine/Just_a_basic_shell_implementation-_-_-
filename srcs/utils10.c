/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 15:17:55 by riramana          #+#    #+#             */
/*   Updated: 2026/01/10 15:18:55 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_error(int *syntax_error)
{
	if (*syntax_error)
		return (2);
	return (1);
}

char	*get_next_token(char *line, int pos)
{
	while (line[pos] && (line[pos] == ' ' || line[pos] == '\t'))
		pos++;
	if (!line[pos])
		return ("newline");
	if (line[pos] == '>' && line[pos + 1] == '>')
		return (">>");
	if (line[pos] == '>')
		return (">");
	if (line[pos] == '<' && line[pos + 1] == '<')
		return ("<<");
	if (line[pos] == '<')
		return ("<");
	if (line[pos] == '|')
		return ("|");
	return ("newline");
}

int	handle_redir_error(char **out_file, char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	if (out_file)
		*out_file = NULL;
	return (0);
}

int	has_pipe(char *command)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (command[i])
	{
		if (command[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (command[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (command[i] == '|' && !in_single_quote && !in_double_quote)
			return (1);
		i++;
	}
	return (0);
}

int	is_echo_command(char *command)
{
	return (ft_strncmp(command, "echo", 4) == 0 && (command[4] == ' '
			|| command[4] == '\t' || command[4] == '\0'));
}
