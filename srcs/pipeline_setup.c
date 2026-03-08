/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:49:52 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 18:49:08 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_alloc_arrays(t_pipeline_data *data)
{
	if (data->cmds)
		free(data->cmds);
	if (data->paths)
		free(data->paths);
	if (data->out_files)
		free(data->out_files);
	if (data->out_modes)
		free(data->out_modes);
	if (data->heredoc_fds)
		free(data->heredoc_fds);
	if (data->input_fds)
		free(data->input_fds);
}

int	allocate_pipeline_arrays(t_pipeline_data *data)
{
	data->cmds = malloc(sizeof(char **) * data->n);
	data->paths = malloc(sizeof(char *) * data->n);
	data->out_files = malloc(sizeof(char *) * data->n);
	data->out_modes = malloc(sizeof(int) * data->n);
	data->heredoc_fds = malloc(sizeof(int) * data->n);
	data->input_fds = malloc(sizeof(int) * data->n);
	if (!data->cmds || !data->paths || !data->out_files || !data->out_modes
		|| !data->heredoc_fds || !data->input_fds)
	{
		free_alloc_arrays(data);
		return (0);
	}
	return (1);
}

int	setup_pipeline(char **cmd_strs, char ****cmds, char ***paths,
		t_pipeline_data *p_data)
{
	t_pipeline_redir	redir;
	int					prep_result;

	if (!allocate_pipeline_arrays(p_data))
	{
		free_split(cmd_strs);
		return (-1);
	}
	*cmds = p_data->cmds;
	*paths = p_data->paths;
	prep_result = prepare_pipeline_data(p_data);
	if (prep_result <= 0)
	{
		assignment_result(p_data, &redir);
		free_pipeline_data(p_data->cmds, p_data->paths, p_data->n);
		free_redir_data(&redir);
		free_split(cmd_strs);
		if (prep_result == -1)
			return (2);
		return (127);
	}
	return (0);
}

static void	free_child_resources(t_exec_data *data, int n, int (*pipes)[2])
{
	int	i;

	close_child_fds(data, n);
	i = 0;
	while (i < n)
	{
		if (data->cmds[i])
			free_split(data->cmds[i]);
		if (data->paths[i])
			free(data->paths[i]);
		if (data->out_files[i])
			free(data->out_files[i]);
		i++;
	}
	free_stru(data);
	if (data->cmd_strs)
		free_split(data->cmd_strs);
	if (data->pids)
		free(data->pids);
	if (data->envp)
		free_split(data->envp);
	if (data->original_cmd)
		free(data->original_cmd);
	free(pipes);
}

void	execute_child(int i, int n, int (*pipes)[2], t_exec_data *data)
{
	int	exit_code;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!data->cmds[i] || !data->paths[i])
	{
		close_all_pipes(pipes, n);
		free_child_resources(data, n, pipes);
		exit(1);
	}
	setup_child_input(i, pipes, data);
	setup_child_output(i, n, pipes, data);
	close_all_pipes(pipes, n);
	if (is_builtin(data->paths[i]))
	{
		exit_code = execute_builtin_in_pipeline(data->paths[i], data->cmds[i],
				data->envp);
		free_child_resources(data, n, pipes);
		exit(exit_code);
	}
	execve(data->paths[i], data->cmds[i], data->envp);
	perror("minishell: execve");
	free_child_resources(data, n, pipes);
	exit(127);
}
