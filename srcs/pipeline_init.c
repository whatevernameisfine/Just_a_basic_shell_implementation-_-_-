/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:49:35 by riramana          #+#    #+#             */
/*   Updated: 2026/01/04 12:44:34 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_pipeline_memory(int (**pipes)[2], pid_t **pids, int n)
{
	*pipes = malloc(sizeof(int [2]) * (n - 1));
	*pids = malloc(sizeof(pid_t) * n);
	if (!(*pipes) || !(*pids))
	{
		if (*pipes)
			free(*pipes);
		if (*pids)
			free(*pids);
		perror("minishell: malloc");
		return (0);
	}
	return (1);
}

int	create_pipes(int (*pipes)[2], int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
			}
			return (0);
		}
		i++;
	}
	return (1);
}

void	cleanup_fork_error(int (*pipes)[2], pid_t *pids, int n)
{
	int	j;

	j = 0;
	while (j < n - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
	free(pipes);
	free(pids);
}

int	wait_all_children(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (i == n - 1)
		{
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					write(2, "Quit (core dumped)\n", 19);
				last_status = 128 + WTERMSIG(status);
			}
			else if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		i++;
	}
	return (last_status);
}
