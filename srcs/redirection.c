/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 09:29:57 by riramana          #+#    #+#             */
/*   Updated: 2026/01/03 07:39:14 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*process_single_redir(char *cleaned, int i, t_redir_data *data)
{
	int		op_len;
	char	*new_cleaned;

	op_len = is_redir_operator(cleaned, i, 0, 0);
	data->should_open = has_more_output_redirections(cleaned, i + op_len);
	new_cleaned = process_redirection(cleaned, &i, data);
	if (new_cleaned)
		free(cleaned);
	return (new_cleaned);
}

static void	cleanup_on_error(char *cleaned, char **out_file)
{
	if (cleaned)
		free(cleaned);
	if (*out_file)
	{
		free(*out_file);
		*out_file = NULL;
	}
}

char	*process_all_redirections(char *cleaned, t_redir_data *data)
{
	char	*new_cleaned;
	int		i;

	while (1)
	{
		i = 0;
		if (!find_next_redir(cleaned, &i))
			break ;
		new_cleaned = process_single_redir(cleaned, i, data);
		if (!new_cleaned || data->has_error)
		{
			cleanup_on_error(cleaned, data->out_file);
			return (NULL);
		}
		cleaned = new_cleaned;
	}
	return (cleaned);
}

char	*parse_output_redirections(char *line, char **out_file, int *out_mode)
{
	char			*cleaned;
	t_redir_data	data;

	*out_file = NULL;
	*out_mode = -1;
	cleaned = ft_strdup(line);
	if (!cleaned)
		return (NULL);
	data = init_redir_data(out_file, out_mode);
	return (process_all_redirections(cleaned, &data));
}

char	*parse_output_redir_with_err(char *line, char **out_file, int *out_mode,
		int *syntax_err)
{
	char			*cleaned;
	t_redir_data	data;

	*out_file = NULL;
	*out_mode = -1;
	if (syntax_err)
		*syntax_err = 0;
	cleaned = ft_strdup(line);
	if (!cleaned)
		return (NULL);
	data = init_redir_data(out_file, out_mode);
	cleaned = process_all_redirections(cleaned, &data);
	if (!cleaned && syntax_err && data.is_syntax_error)
		*syntax_err = 1;
	return (cleaned);
}
