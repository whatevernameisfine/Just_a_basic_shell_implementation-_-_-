/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:58:06 by riramana          #+#    #+#             */
/*   Updated: 2026/03/08 11:22:20 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_exit_builtin(char **cmd_args, t_shell_data *data)
{
	int	exit_code;

	write(1, "exit\n", 5);
	if (cmd_args[1] && cmd_args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (cmd_args[1] && !is_numeric_string(cmd_args[1]))
	{
		msg_exit_builtin(cmd_args[1]);
		free_split(cmd_args);
		if (data->current_cmd)
			free(data->current_cmd);
		free_split(data->envp);
		exit(2);
	}
	exit_code = data->last_exit_status;
	if (cmd_args[1])
		exit_code = ft_atoi(cmd_args[1]);
	free_split(cmd_args);
	if (data->current_cmd)
		free(data->current_cmd);
	free_split(data->envp);
	exit(exit_code);
}

int	execute_builtin(char **cmd_args, t_shell_data *data)
{
	int	ret;

	if (ft_strcmp(cmd_args[0], ".") == 0)
		return (handle_dot_builtin(cmd_args));
	if (ft_strcmp(cmd_args[0], "exit") == 0)
		return (handle_exit_builtin(cmd_args, data));
	if (ft_strcmp(cmd_args[0], "cd") == 0)
		return (handle_cd_command(cmd_args, data));
	if (ft_strcmp(cmd_args[0], "export") == 0)
		ret = handle_export_command(data, cmd_args);
	else if (ft_strcmp(cmd_args[0], "unset") == 0)
		ret = handle_unset_command(data, cmd_args);
	else if (ft_strcmp(cmd_args[0], "pwd") == 0)
		ret = handle_pwd_command();
	else if (ft_strcmp(cmd_args[0], "env") == 0)
		ret = handle_env_command(data->envp);
	else if (ft_strcmp(cmd_args[0], "minifetch") == 0)
		ret = handle_neofetch();
	else
		return (-1);
	return (ret);
}

static int	exec_builtin_with_redirect(char **cmd_args, t_shell_data *data,
		char *out_file, int out_mode)
{
	int	ret;
	int	saved_stdout;

	saved_stdout = -1;
	if (out_file && out_mode != -1)
	{
		if (setup_builtin_redirect(out_file, out_mode, &saved_stdout) == -1)
			return (-1);
	}
	ret = execute_builtin(cmd_args, data);
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	return (ret);
}

int	handle_builtin_command(char *command, t_shell_data *data)
{
	char			**cmd_args;
	int				ret;
	t_builtin_redir	redir;

	redir.syntax_err = 0;
	cmd_args = prepare_builtin_args_ext(command, &redir, data);
	if (!cmd_args || !cmd_args[0])
	{
		if (cmd_args)
			free_split(cmd_args);
		if (redir.out_file)
			free(redir.out_file);
		if (!cmd_args && redir.syntax_err)
			return (2);
		return (!cmd_args);
	}
	ret = exec_builtin_with_redirect(cmd_args, data, redir.out_file,
			redir.out_mode);
	free_split(cmd_args);
	if (redir.out_file)
		free(redir.out_file);
	if (ret == -1)
		return (-1);
	return (ret);
}

int	execute_command(char *command, t_shell_data *data)
{
	int	ret;

	if (has_pipe(command))
		return (handle_pipeline_command(command, data));
	if (is_echo_command(command))
		return (handle_echo_cmd(command, data));
	if (!has_input_redirection(command))
	{
		ret = handle_builtin_command(command, data);
		if (ret >= 0)
			return (ret);
	}
	return (handle_external_cmd(command, data));
}
