/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 06:55:13 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 16:16:43 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_pipeline_command(char *command, t_shell_data *data)
{
	if (has_redir_before_pipe(command, 0, 0))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (2);
	}
	if (!validate_pipe_syntax(command))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (2);
	}
	return (execute_pipeline_wrapper(command, data));
}

int	fork_and_execute(int i, int n, int (*pipes)[2], t_exec_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		execute_child(i, n, pipes, data);
	close_parent_pipes(i, pipes);
	return (pid);
}

int	is_numeric_string(char *str)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] != '\0' || check_overflow(str, sign))
		return (0);
	return (1);
}

char	*process_input_redirection(char *line, int *i, int *heredoc_fd,
		int *input_fd)
{
	int		op_len;
	int		redir_start;
	char	*result;

	op_len = is_input_redir_operator(line, *i, 0, 0);
	redir_start = *i;
	*i += op_len;
	if (op_len == 2)
		result = handle_heredoc_redir(line, i, heredoc_fd, input_fd);
	else
		result = handle_input_file_redir(line, i, heredoc_fd, input_fd);
	if (result == (char *)-1)
		return ((char *)-1);
	if (!result)
		return (NULL);
	return (build_cleaned_cmd(line, redir_start, *i));
}

char	*process_redirection(char *line, int *i, t_redir_data *data)
{
	int		op_len;
	int		redir_start;
	char	*filename;
	int		mode;

	op_len = is_redir_operator(line, *i, 0, 0);
	redir_start = *i;
	*i += op_len;
	filename = handle_file_extraction(line, i, data->out_file);
	if (!filename)
	{
		data->is_syntax_error = 1;
		return (NULL);
	}
	mode = get_redir_mode(op_len);
	handle_filename_cases(filename, data, mode);
	if (data->has_error)
		return (NULL);
	return (build_cleaned_cmd(line, redir_start, *i));
}
