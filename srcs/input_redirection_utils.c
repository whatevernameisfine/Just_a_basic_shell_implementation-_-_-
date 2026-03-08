/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 06:11:48 by riramana          #+#    #+#             */
/*   Updated: 2026/01/16 17:32:18 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*strip_quotes_if_empty(char *str)
{
	int		len;

	len = ft_strlen(str);
	if (len >= 2)
	{
		if ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"'))
		{
			if (len == 2)
				return (ft_strdup(""));
			return (ft_substr(str, 1, len - 2));
		}
	}
	return (ft_strdup(str));
}

static int	is_delim_end(char c, char in_quote)
{
	if (c == '\0')
		return (1);
	if (in_quote)
		return (0);
	if (c == ' ' || c == '\t' || c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

char	*extract_heredoc_delimiter(char *line, int *pos)
{
	int		start;
	int		len;
	char	*delimiter;
	char	*raw_delim;
	char	in_quote;

	while (line[*pos] && (line[*pos] == ' ' || line[*pos] == '\t'))
		(*pos)++;
	start = *pos;
	in_quote = 0;
	while (!is_delim_end(line[*pos], in_quote))
	{
		if ((line[*pos] == '\'' || line[*pos] == '"') && !in_quote)
			in_quote = line[*pos];
		else if (line[*pos] == in_quote)
			in_quote = 0;
		(*pos)++;
	}
	len = *pos - start;
	if (len == 0)
		return (NULL);
	raw_delim = ft_substr(line, start, len);
	delimiter = strip_quotes_if_empty(raw_delim);
	free(raw_delim);
	return (delimiter);
}

char	*process_single_input_redir(char *result, int *i, int *heredoc_fd,
		int *input_fd)
{
	char	*new_result;

	new_result = process_input_redirection(result, i, heredoc_fd, input_fd);
	free(result);
	if (new_result == (char *)-1)
		return ((char *)-1);
	if (!new_result)
		return (NULL);
	*i = 0;
	return (new_result);
}

char	*parse_input_redirections(char *line, int *heredoc_fd, int *input_fd)
{
	char	*result;

	*heredoc_fd = -1;
	*input_fd = -1;
	result = ft_strdup(line);
	result = parse_input_loop(result, heredoc_fd, input_fd);
	if (result == (char *)-1)
		return (NULL);
	return (result);
}
