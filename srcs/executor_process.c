/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 12:45:57 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:31:05 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	setup_child_output_redirect(char *out_file, int out_mode)
{
	int	fd;

	fd = open(out_file, out_mode, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(out_file, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	setup_child_redirects(t_child_exec *data)
{
	if (data->input_fd != -1)
	{
		if (dup2(data->input_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(data->input_fd);
	}
	else if (data->heredoc_fd != -1)
	{
		if (dup2(data->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(data->heredoc_fd);
	}
	if (data->out_file && data->out_mode != -1)
		setup_child_output_redirect(data->out_file, data->out_mode);
}

int	execute_child_process(t_child_exec *data)
{
	struct stat	path_stat;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_redirects(data);
	if (stat(data->exec_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(data->exec_path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	if (execve(data->exec_path, data->cmd_args, data->envp) == -1)
	{
		perror("execve failed");
		exit(1);
	}
	return (0);
}

int	handle_parent_process(char **cmd_args)
{
	int	status;

	wait(&status);
	free_split(cmd_args);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (130);
		if (WTERMSIG(status) == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 19);
			return (131);
		}
		return (128 + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}
