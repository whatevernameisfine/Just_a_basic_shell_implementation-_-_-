/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:49:10 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:23:12 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	execute_all_commands(int n, int (*pipes)[2], t_exec_data *data,
		pid_t *pids)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pids[i] = fork_and_execute(i, n, pipes, data);
		if (pids[i] == -1)
		{
			perror("minishell: fork");
			cleanup_fork_error(pipes, pids, n);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	exec_pipeline_impl(t_pipeline_data *p_data)
{
	pid_t		*pids;
	t_exec_data	data;
	int			exit_status;
	int			(*pipes)[2];

	if (!init_pipeline_memory(&pipes, &pids, p_data->n))
		return (1);
	if (!create_pipes(pipes, p_data->n))
	{
		free(pipes);
		free(pids);
		return (1);
	}
	init_exec_data(&data, p_data);
	data.pids = pids;
	if (!execute_all_commands(p_data->n, pipes, &data, pids))
		return (1);
	close_parent_pipe_ends(pipes, p_data->n);
	exit_status = wait_all_children(pids, p_data->n);
	free(pipes);
	free(pids);
	return (exit_status);
}

int	execute_pipeline(t_pipeline_data *p_data)
{
	return (exec_pipeline_impl(p_data));
}

static void	cleanup_pipeline_resources(t_pipeline_data *p_data, char ***cmds,
		char **paths)
{
	t_pipeline_redir	redir;

	free_pipeline_data(cmds, paths, p_data->n);
	redir.out_files = p_data->out_files;
	redir.out_modes = p_data->out_modes;
	redir.heredoc_fds = p_data->heredoc_fds;
	redir.input_fds = p_data->input_fds;
	redir.n = p_data->n;
	free_redir_data(&redir);
	free_split(p_data->cmd_strs);
}

int	execute_pipeline_wrapper(char *command, t_shell_data *data)
{
	t_pipeline_data	p_data;
	char			***cmds;
	char			**paths;
	int				ret;

	p_data.n = count_pipes(command) + 1;
	p_data.cmd_strs = split_pipeline(command);
	p_data.envp = data->envp;
	p_data.last_exit_status = data->last_exit_status;
	p_data.original_cmd = command;
	if (!p_data.cmd_strs)
		return (-1);
	if (!validate_pipeline_syntax(p_data.cmd_strs, p_data.n))
	{
		free_split(p_data.cmd_strs);
		return (2);
	}
	ret = setup_pipeline(p_data.cmd_strs, &cmds, &paths, &p_data);
	if (ret != 0)
		return (ret);
	ret = execute_pipeline(&p_data);
	cleanup_pipeline_resources(&p_data, cmds, paths);
	return (ret);
}
