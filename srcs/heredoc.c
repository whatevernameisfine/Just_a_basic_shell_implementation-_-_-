/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 12:47:14 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:49:03 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_delimiter_match(char *line, char *delimiter, int is_empty)
{
	if (is_empty && line[0] == '\0')
		return (1);
	if (ft_strcmp(line, delimiter) == 0)
		return (1);
	return (0);
}

static int	check_heredoc_signal(void)
{
	if (g_signal == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

static int	process_heredoc_line(char *line, char *delimiter,
		int is_empty_delim, int pipe_fd)
{
	if (g_signal == SIGINT)
	{
		if (line)
			free(line);
		return (-1);
	}
	if (!line)
	{
		print_heredoc_warning(delimiter);
		return (1);
	}
	if (check_delimiter_match(line, delimiter, is_empty_delim))
	{
		free(line);
		return (1);
	}
	ft_putstr_fd(line, pipe_fd);
	ft_putstr_fd("\n", pipe_fd);
	free(line);
	return (0);
}

static int	write_to_pipe(int pipe_fd, char *delimiter)
{
	char	*line;
	int		is_empty_delim;
	int		status;

	is_empty_delim = (delimiter[0] == '\0');
	rl_event_hook = check_heredoc_signal;
	while (1)
	{
		line = readline("> ");
		status = process_heredoc_line(line, delimiter, is_empty_delim, pipe_fd);
		if (status == -1)
		{
			rl_event_hook = NULL;
			return (-1);
		}
		if (status == 1)
			break ;
	}
	rl_event_hook = NULL;
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int	pipe_fds[2];
	int	status;

	if (pipe(pipe_fds) == -1)
		return (-1);
	status = write_to_pipe(pipe_fds[1], delimiter);
	close(pipe_fds[1]);
	if (status == -1)
	{
		close(pipe_fds[0]);
		return (-1);
	}
	return (pipe_fds[0]);
}
