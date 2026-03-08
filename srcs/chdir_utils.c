/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 09:29:06 by riramana          #+#    #+#             */
/*   Updated: 2026/01/05 13:59:44 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_target_dir(char **cmd_args, char **envp)
{
	char	*target_dir;

	if (cmd_args[1] && cmd_args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (NULL);
	}
	if (cmd_args[1] == NULL || ft_strcmp(cmd_args[1], "~") == 0)
	{
		target_dir = get_env_value(envp, "HOME");
		if (target_dir == NULL)
			ft_putstr_fd("cd: HOME not set\n", 2);
		return (target_dir);
	}
	return (cmd_args[1]);
}

int	handle_cd_command(char **cmd_args, t_shell_data *data)
{
	char	*target_dir;
	char	*old_pwd;
	char	cwd[1024];

	target_dir = get_target_dir(cmd_args, data->envp);
	if (!target_dir)
		return (1);
	old_pwd = getcwd(cwd, sizeof(cwd));
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		return (1);
	}
	if (old_pwd)
		data->envp = set_env_value(data->envp, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		data->envp = set_env_value(data->envp, "PWD", cwd);
	return (0);
}

static int	execute_in_child(char *exec_path, t_child_exec *child_data)
{
	child_data->exec_path = exec_path;
	return (execute_child_process(child_data));
}

static void	init_child_data(t_child_exec *child_data, t_exec_params *params)
{
	child_data->cmd_args = params->cmd_args;
	child_data->envp = params->envp;
	child_data->out_file = params->out_file;
	child_data->out_mode = params->out_mode;
	child_data->heredoc_fd = params->heredoc_fd;
	child_data->input_fd = params->input_fd;
}

int	execute_external_command(t_exec_params *params)
{
	pid_t			pid;
	char			*exec_path;
	t_child_exec	child_data;

	exec_path = find_executable_path(params->cmd_args[0], params->envp);
	if (!exec_path)
		return (handle_command_not_found(params->cmd_args));
	pid = fork();
	if (pid < 0)
		return (handle_fork_error(params->cmd_args, exec_path));
	if (pid == 0)
	{
		init_child_data(&child_data, params);
		return (execute_in_child(exec_path, &child_data));
	}
	if (params->heredoc_fd != -1)
		close(params->heredoc_fd);
	if (params->input_fd != -1)
		close(params->input_fd);
	free(exec_path);
	return (handle_parent_process(params->cmd_args));
}
