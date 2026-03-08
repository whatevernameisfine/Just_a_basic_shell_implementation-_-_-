/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_prep.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:49:43 by riramana          #+#    #+#             */
/*   Updated: 2026/01/08 13:42:27 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_arrays(t_pipeline_data *data)
{
	int	i;

	i = 0;
	while (i < data->n)
	{
		data->cmds[i] = NULL;
		data->paths[i] = NULL;
		data->out_files[i] = NULL;
		data->out_modes[i] = -1;
		data->heredoc_fds[i] = -1;
		data->input_fds[i] = -1;
		i++;
	}
}

static char	*handle_output_and_expand(char *cleaned_input,
		t_pipeline_data *data, int i, int *syntax_err)
{
	char			*cleaned_output;
	char			*expanded;
	t_shell_data	shell_data;

	cleaned_output = parse_output_redir_with_err(cleaned_input,
			&data->out_files[i], &data->out_modes[i], syntax_err);
	free(cleaned_input);
	if (!cleaned_output)
	{
		if (*syntax_err)
			return ((char *)-1);
		return (NULL);
	}
	shell_data.envp = data->envp;
	shell_data.last_exit_status = data->last_exit_status;
	if (!expand_redir_filename(&data->out_files[i], data->out_modes[i],
			&shell_data))
	{
		free(cleaned_output);
		return (NULL);
	}
	expanded = expand_command_line(cleaned_output, &shell_data);
	free(cleaned_output);
	return (expanded);
}

static char	*parse_and_expand(char *cmd_str, t_pipeline_data *data, int i)
{
	char	*trimmed;
	char	*cleaned_input;
	int		syntax_err;

	trimmed = trim_whitespace(cmd_str);
	if (!trimmed)
		return (NULL);
	syntax_err = 0;
	cleaned_input = parse_input_redir_with_err(trimmed, &data->heredoc_fds[i],
			&data->input_fds[i], &syntax_err);
	free(trimmed);
	if (!cleaned_input)
	{
		if (syntax_err)
			return ((char *)-1);
		return (NULL);
	}
	return (handle_output_and_expand(cleaned_input, data, i, &syntax_err));
}

static int	process_command(char *cmd_str, t_pipeline_data *data, int i)
{
	char	*expanded;

	expanded = parse_and_expand(cmd_str, data, i);
	if (expanded == (char *)-1)
	{
		init_stru_data(data, i);
		return (-1);
	}
	if (!expanded)
	{
		init_stru_data(data, i);
		return (1);
	}
	data->cmds[i] = split_echo_args(expanded);
	free(expanded);
	if (!data->cmds[i] || !data->cmds[i][0])
	{
		data->paths[i] = NULL;
		return (1);
	}
	if (is_builtin(data->cmds[i][0]))
		data->paths[i] = ft_strdup(data->cmds[i][0]);
	else
		data->paths[i] = find_executable_path(data->cmds[i][0], data->envp);
	return (process_cmd_helper(data->cmds, data->paths, i));
}

int	prepare_pipeline_data(t_pipeline_data *data)
{
	int	i;
	int	ret;

	init_arrays(data);
	i = 0;
	while (i < data->n)
	{
		ret = process_command(data->cmd_strs[i], data, i);
		if (ret == -1)
			return (-1);
		if (ret == 0)
			return (0);
		i++;
	}
	return (1);
}
