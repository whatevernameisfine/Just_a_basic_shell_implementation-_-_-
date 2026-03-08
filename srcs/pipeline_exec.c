/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:49:26 by riramana          #+#    #+#             */
/*   Updated: 2025/12/13 18:08:24 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	dup_and_close(int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(fd);
}

void	setup_child_input(int i, int (*pipes)[2], t_exec_data *data)
{
	if (data->input_fds[i] != -1)
	{
		dup_and_close(data->input_fds[i]);
		return ;
	}
	if (data->heredoc_fds[i] != -1)
	{
		dup_and_close(data->heredoc_fds[i]);
		return ;
	}
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
	}
}

static void	setup_output_file(int i, t_exec_data *data)
{
	int	fd;

	fd = open(data->out_files[i], data->out_modes[i], 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(data->out_files[i], 2);
		ft_putstr_fd(": ", 2);
		perror("");
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	setup_child_output(int i, int n, int (*pipes)[2], t_exec_data *data)
{
	if (data->out_files[i] && data->out_modes[i] != -1)
		setup_output_file(i, data);
	else if (i < n - 1)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
	}
}

void	close_all_pipes(int (*pipes)[2], int n)
{
	int	j;

	j = 0;
	while (j < n - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}
