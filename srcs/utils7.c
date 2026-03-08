/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 07:19:18 by riramana          #+#    #+#             */
/*   Updated: 2026/01/03 07:39:23 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*parse_input_redir_with_err(char *line, int *heredoc_fd, int *input_fd,
		int *syntax_err)
{
	char	*result;

	*heredoc_fd = -1;
	*input_fd = -1;
	if (syntax_err)
		*syntax_err = 0;
	result = ft_strdup(line);
	result = parse_input_loop(result, heredoc_fd, input_fd);
	if (result == (char *)-1)
	{
		if (syntax_err)
			*syntax_err = 1;
		return (NULL);
	}
	return (result);
}

int	process_cmd_helper(char ***cmds, char **paths, int i)
{
	if (!paths[i])
	{
		if (is_builtin(cmds[i][0]))
			paths[i] = ft_strdup(cmds[i][0]);
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmds[i][0], 2);
			ft_putstr_fd(": command not found\n", 2);
			return (0);
		}
	}
	return (1);
}

int	init_exec_data(t_exec_data *data, t_pipeline_data *p_data)
{
	data->cmds = p_data->cmds;
	data->paths = p_data->paths;
	data->envp = p_data->envp;
	data->out_files = p_data->out_files;
	data->out_modes = p_data->out_modes;
	data->heredoc_fds = p_data->heredoc_fds;
	data->input_fds = p_data->input_fds;
	data->cmd_strs = p_data->cmd_strs;
	data->n = p_data->n;
	data->pids = NULL;
	data->original_cmd = p_data->original_cmd;
	return (1);
}

void	expand_args(char **args, t_shell_data *data)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args[i])
	{
		expanded = expand_string(args[i], data);
		if (expanded)
		{
			free(args[i]);
			args[i] = expanded;
		}
		i++;
	}
}

int	find_next_redir(char *cleaned, int *i)
{
	int	in_sq;
	int	in_dq;
	int	op_len;

	in_sq = 0;
	in_dq = 0;
	while (cleaned[*i])
	{
		skip_quotes(cleaned, i, &in_sq, &in_dq);
		op_len = is_redir_operator(cleaned, *i, in_sq, in_dq);
		if (op_len)
			return (op_len);
		if (cleaned[*i])
			(*i)++;
	}
	return (0);
}
