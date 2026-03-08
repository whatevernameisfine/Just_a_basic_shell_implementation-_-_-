/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 06:11:57 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 16:27:21 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_and_validate_delimiter(char *line, int *i)
{
	char	*delimiter;
	char	*token;

	delimiter = extract_heredoc_delimiter(line, i);
	if (!delimiter)
	{
		token = get_next_token(line, *i);
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("'\n", 2);
		return (NULL);
	}
	return (delimiter);
}

static void	close_existing_fds(int *heredoc_fd, int *input_fd)
{
	if (*heredoc_fd != -1)
	{
		close(*heredoc_fd);
		*heredoc_fd = -1;
	}
	if (*input_fd != -1)
		close(*input_fd);
}

char	*handle_heredoc_redir(char *line, int *i, int *heredoc_fd,
		int *input_fd)
{
	char	*filename;

	filename = get_and_validate_delimiter(line, i);
	if (!filename)
		return ((char *)-1);
	close_existing_fds(heredoc_fd, input_fd);
	*heredoc_fd = handle_heredoc(filename);
	*input_fd = -1;
	free(filename);
	if (*heredoc_fd == -1 && g_signal == SIGINT)
		return (NULL);
	return ((char *)1);
}

char	*handle_input_file_redir(char *line, int *i, int *heredoc_fd,
		int *input_fd)
{
	char	*filename;

	filename = extract_filename_with_error(line, i, NULL);
	if (!filename)
		return ((char *)-1);
	close_existing_fds(heredoc_fd, input_fd);
	*input_fd = open_input_file(filename);
	free(filename);
	if (*input_fd == -1)
		return (NULL);
	return ((char *)1);
}

char	*parse_input_loop(char *result, int *heredoc_fd, int *input_fd)
{
	int	i;
	int	in_sq;
	int	in_dq;
	int	op_len;

	i = init_two(&in_sq, &in_dq);
	while (result && result[i])
	{
		skip_quotes(result, &i, &in_sq, &in_dq);
		op_len = is_input_redir_operator(result, i, in_sq, in_dq);
		if (op_len)
		{
			result = process_single_input_redir(result, &i, heredoc_fd,
					input_fd);
			if (result == (char *)-1)
				return ((char *)-1);
			if (!result)
				return (NULL);
			in_sq = 0;
			in_dq = 0;
		}
		else if (result[i])
			i++;
	}
	return (result);
}
