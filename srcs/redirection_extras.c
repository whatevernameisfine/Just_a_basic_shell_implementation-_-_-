/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_extras.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:42:13 by riramana          #+#    #+#             */
/*   Updated: 2026/01/16 17:33:14 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_more_output_redirections(char *line, int start_pos)
{
	int	i;
	int	in_sq;
	int	in_dq;

	i = start_pos;
	in_sq = 0;
	in_dq = 0;
	while (line[i])
	{
		skip_quotes(line, &i, &in_sq, &in_dq);
		if (is_redir_operator(line, i, in_sq, in_dq))
			return (1);
		if (line[i])
			i++;
	}
	return (0);
}

int	get_redir_mode(int op_len)
{
	int	mode;

	if (op_len == 2)
		mode = O_WRONLY | O_CREAT | O_APPEND;
	else
		mode = O_WRONLY | O_CREAT | O_TRUNC;
	return (mode);
}

char	*handle_file_extraction(char *line, int *i, char **out_file)
{
	return (extract_filename_with_error(line, i, out_file));
}

int	process_redir_opening(char *filename, char **out_file, int mode,
		int *has_error)
{
	if (*out_file)
		free(*out_file);
	if (open_and_check_file(filename, mode) == -1)
	{
		*has_error = 1;
		*out_file = NULL;
		free(filename);
		return (0);
	}
	*out_file = filename;
	return (1);
}

int	expand_redir_filename(char **filename, int mode, t_shell_data *data)
{
	char	*expanded;

	if (!*filename)
		return (1);
	expanded = expand_string(*filename, data);
	if (!expanded)
		return (0);
	free(*filename);
	*filename = expanded;
	if (open_and_check_file(*filename, mode) == -1)
	{
		free(*filename);
		*filename = NULL;
		return (0);
	}
	return (1);
}
