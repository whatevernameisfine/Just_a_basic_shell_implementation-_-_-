/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 06:50:48 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:23:07 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	setup_builtin_redirect(char *out_file, int out_mode, int *saved_stdout)
{
	int	fd;

	fd = open(out_file, out_mode, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(out_file, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (-1);
	}
	*saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

char	**prepare_builtin_args(char *command, char **out_file, int *out_mode)
{
	t_builtin_redir	redir;
	char			**result;

	redir.syntax_err = 0;
	result = prepare_builtin_args_ext(command, &redir, NULL);
	*out_file = redir.out_file;
	*out_mode = redir.out_mode;
	return (result);
}

char	**prepare_builtin_args_with_data(char *command, char **out_file,
		int *out_mode, t_shell_data *data)
{
	t_builtin_redir	redir;
	char			**result;

	redir.syntax_err = 0;
	result = prepare_builtin_args_ext(command, &redir, data);
	*out_file = redir.out_file;
	*out_mode = redir.out_mode;
	return (result);
}

char	**prepare_builtin_args_ext(char *command, t_builtin_redir *redir,
		t_shell_data *data)
{
	char	*cleaned;
	char	**cmd_args;

	cleaned = parse_output_redir_with_err(command, &redir->out_file,
			&redir->out_mode, &redir->syntax_err);
	if (!cleaned)
		return (NULL);
	if (data && !expand_redir_filename(&redir->out_file, redir->out_mode, data))
	{
		free(cleaned);
		return (NULL);
	}
	if (data)
		cmd_args = split_echo_args_with_data(cleaned, data);
	else
		cmd_args = split_echo_args(cleaned);
	free(cleaned);
	return (cmd_args);
}
