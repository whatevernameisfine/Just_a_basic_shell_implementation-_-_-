/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 12:47:32 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:32:19 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_out_files(char **out_files, int n)
{
	int	i;

	if (!out_files)
		return ;
	i = 0;
	while (i < n)
	{
		if (out_files[i])
			free(out_files[i]);
		i++;
	}
	free(out_files);
}

static void	close_and_free_heredocs(int *heredoc_fds, int n)
{
	int	i;

	if (!heredoc_fds)
		return ;
	i = 0;
	while (i < n)
	{
		if (heredoc_fds[i] != -1)
			close(heredoc_fds[i]);
		i++;
	}
	free(heredoc_fds);
}

static void	close_and_free_inputs(int *input_fds, int n)
{
	int	i;

	if (!input_fds)
		return ;
	i = 0;
	while (i < n)
	{
		if (input_fds[i] != -1)
			close(input_fds[i]);
		i++;
	}
	free(input_fds);
}

void	free_redir_data(t_pipeline_redir *redir)
{
	free_out_files(redir->out_files, redir->n);
	if (redir->out_modes)
		free(redir->out_modes);
	close_and_free_heredocs(redir->heredoc_fds, redir->n);
	close_and_free_inputs(redir->input_fds, redir->n);
}

void	close_parent_pipe_ends(int (*pipes)[2], int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][1]);
		i++;
	}
}
