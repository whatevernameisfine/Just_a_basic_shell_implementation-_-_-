/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 12:46:21 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:49:46 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_quoted_filename(char *str, int *pos)
{
	char	quote;
	int		start;
	int		i;
	char	*content;

	quote = str[*pos];
	start = *pos + 1;
	i = start;
	while (str[i] && str[i] != quote)
		i++;
	*pos = i + 1;
	content = ft_substr(str, start, i - start);
	return (content);
}

char	*extract_unquoted_filename(char *str, int *pos)
{
	int		start;
	int		i;
	char	*content;

	start = *pos;
	i = start;
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\''
		&& str[i] != '"' && str[i] != '>' && str[i] != '<' && str[i] != '|')
		i++;
	*pos = i;
	content = ft_substr(str, start, i - start);
	return (content);
}

char	*extract_filename(char *line, int *pos)
{
	char	*result;
	int		start_pos;

	while (line[*pos] && (line[*pos] == ' ' || line[*pos] == '\t'))
		(*pos)++;
	start_pos = *pos;
	result = build_filename_parts(line, pos);
	if (*pos == start_pos)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

int	has_input_redirection(char *command)
{
	int	i;
	int	in_sq;
	int	in_dq;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (command[i])
	{
		skip_quotes(command, &i, &in_sq, &in_dq);
		if (command[i] && !in_sq && !in_dq && command[i] == '<')
			return (1);
		if (command[i])
			i++;
	}
	return (0);
}

int	output_comes_before_input(char *command)
{
	int	i;
	int	in_sq;
	int	in_dq;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (command[i])
	{
		skip_quotes(command, &i, &in_sq, &in_dq);
		if (command[i] && !in_sq && !in_dq)
		{
			if (command[i] == '>')
				return (1);
			if (command[i] == '<')
				return (0);
		}
		if (command[i])
			i++;
	}
	return (0);
}
