/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:13:41 by riramana          #+#    #+#             */
/*   Updated: 2026/01/16 17:31:39 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*build_cleaned_cmd(char *line, int redir_start, int redir_end)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(line, 0, redir_start);
	after = ft_strdup(line + redir_end);
	if (!before || !after)
	{
		free(before);
		free(after);
		return (NULL);
	}
	temp = ft_strjoin(before, after);
	free(before);
	free(after);
	if (!temp)
		return (NULL);
	result = trim_whitespace(temp);
	free(temp);
	return (result);
}

char	*extract_filename_with_error(char *line, int *pos, char **out_file)
{
	char	*filename;
	char	*token;

	filename = extract_filename(line, pos);
	if (!filename)
	{
		token = get_next_token(line, *pos);
		handle_redir_error(out_file, token);
		return (NULL);
	}
	return (filename);
}

static int	needs_expansion(char *filename)
{
	int	i;

	i = 0;
	while (filename[i])
	{
		if (filename[i] == '$' && filename[i + 1]
			&& (filename[i + 1] == '?' || is_valid_var_char(filename[i + 1])))
			return (1);
		i++;
	}
	return (0);
}

void	handle_filename_cases(char *filename, t_redir_data *data, int mode)
{
	if (data->has_error)
	{
		free(filename);
		return ;
	}
	if (needs_expansion(filename))
	{
		if (*data->out_file)
			free(*data->out_file);
		*data->out_file = filename;
	}
	else
		process_redir_opening(filename, data->out_file, mode, &data->has_error);
	if (!data->has_error)
		*data->out_mode = mode;
}

t_redir_data	init_redir_data(char **out_file, int *out_mode)
{
	t_redir_data	data;

	data.out_file = out_file;
	data.out_mode = out_mode;
	data.should_open = 0;
	data.has_error = 0;
	data.is_syntax_error = 0;
	return (data);
}
