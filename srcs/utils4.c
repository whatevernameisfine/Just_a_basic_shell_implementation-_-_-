/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 18:39:06 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:03:23 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_empty_cmd_args(char **cmd_args, char *out_file,
		int heredoc_fd, int input_fd)
{
	if (cmd_args)
		free_split(cmd_args);
	if (out_file)
		free(out_file);
	if (heredoc_fd != -1)
		close(heredoc_fd);
	if (input_fd != -1)
		close(input_fd);
	return (0);
}

static char	*parse_redirections(char *command, t_exec_params *params,
		int *syntax_err)
{
	char	*cleaned_second;

	if (output_comes_before_input(command))
	{
		if (!is_out_or_in(command, params, syntax_err,
				&cleaned_second))
			return (NULL);
	}
	else
	{
		if (!is_in_or_out(command, params, syntax_err,
				&cleaned_second))
			return (NULL);
	}
	return (cleaned_second);
}

static char	**parse_and_expand_cmd(char *command, t_shell_data *data,
		t_exec_params *params, int *syntax_err)
{
	char	*cleaned_second;
	char	**cmd_args;

	cleaned_second = parse_redirections(command, params, syntax_err);
	if (!cleaned_second)
		return (NULL);
	cmd_args = split_echo_args_with_data(cleaned_second, data);
	free(cleaned_second);
	return (cmd_args);
}

void	cleanup_params(t_exec_params *params)
{
	if (params->out_file)
		free(params->out_file);
	if (params->heredoc_fd != -1)
		close(params->heredoc_fd);
	if (params->input_fd != -1)
		close(params->input_fd);
}

int	handle_external_cmd(char *command, t_shell_data *data)
{
	t_exec_params	params;
	char			**cmd_args;
	int				ret;
	int				syntax_err;

	syntax_err = 0;
	init_exec_param(&params);
	cmd_args = parse_and_expand_cmd(command, data, &params, &syntax_err);
	if (!cmd_args)
		return (external_ret(&params, syntax_err));
	if (!expand_redir_filename(&params.out_file, params.out_mode, data))
	{
		free_split(cmd_args);
		cleanup_params(&params);
		return (1);
	}
	if (!cmd_args[0])
		return (handle_empty_cmd_args(cmd_args, params.out_file,
				params.heredoc_fd, params.input_fd));
	params.cmd_args = cmd_args;
	params.envp = data->envp;
	ret = execute_external_command(&params);
	if (params.out_file)
		free(params.out_file);
	return (ret);
}
