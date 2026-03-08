/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 12:46:55 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:09:52 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	open_output_redirect(char *out_file, int out_mode,
		int *saved_stdout)
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

static void	restore_and_cleanup(int saved_stdout, char *cleaned, char *out_file)
{
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	free(cleaned);
	if (out_file)
		free(out_file);
}

static int	cleanup_echo_resources(char *cleaned, char *out_file,
		int heredoc_fd, int input_fd)
{
	if (cleaned)
		free(cleaned);
	if (out_file)
		free(out_file);
	if (heredoc_fd != -1)
		close(heredoc_fd);
	if (input_fd != -1)
		close(input_fd);
	return (1);
}

static char	*parse_echo_redirections(char *command, t_echo_redir *redir,
		int *syntax_err)
{
	char	*cleaned_input;
	char	*cleaned;
	int		input_syntax_err;
	int		output_syntax_err;

	input_syntax_err = 0;
	output_syntax_err = 0;
	cleaned_input = parse_input_redir_with_err(command, &redir->heredoc_fd,
			&redir->input_fd, &input_syntax_err);
	if (!cleaned_input)
	{
		*syntax_err = input_syntax_err;
		return (NULL);
	}
	cleaned = parse_output_redir_with_err(cleaned_input, &redir->out_file,
			&redir->out_mode, &output_syntax_err);
	free(cleaned_input);
	if (!cleaned)
	{
		*syntax_err = output_syntax_err;
		cleanup_echo_resources(NULL, NULL, redir->heredoc_fd, redir->input_fd);
		return (NULL);
	}
	*syntax_err = 0;
	return (cleaned);
}

int	handle_echo_cmd(char *command, t_shell_data *data)
{
	char			*cleaned;
	t_echo_redir	redir;
	int				ret;
	int				saved_stdout;
	int				syntax_err;

	init_echo_redir(&redir);
	cleaned = parse_echo_redirections(command, &redir, &syntax_err);
	if (!cleaned)
		return (syntax_error(&syntax_err));
	if (!expand_redir_filename(&redir.out_file, redir.out_mode, data))
		return (cleanup_echo_resources(cleaned, NULL, redir.heredoc_fd,
				redir.input_fd));
	saved_stdout = -1;
	if (redir.out_file && redir.out_mode != -1)
	{
		if (open_output_redirect(redir.out_file, redir.out_mode,
				&saved_stdout) == -1)
			return (cleanup_echo_resources(cleaned, redir.out_file,
					redir.heredoc_fd, redir.input_fd));
	}
	ret = handle_echo_command(cleaned, data);
	restore_and_cleanup(saved_stdout, cleaned, redir.out_file);
	close_echo_fds(&redir);
	return (ret);
}
