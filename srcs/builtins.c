/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 15:54:08 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 18:35:29 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_echo_in_pipeline(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		if (write(1, args[i], ft_strlen(args[i])) == -1)
			return (0);
		if (args[i + 1] && write(1, " ", 1) == -1)
			return (0);
		i++;
	}
	if (newline && write(1, "\n", 1) == -1)
		return (0);
	return (0);
}

static int	handle_export_in_pipeline(char **cmd_args)
{
	int		i;
	char	*equal_sign;
	char	*name;

	if (!cmd_args[1])
		return (0);
	i = 1;
	while (cmd_args[i])
	{
		equal_sign = ft_strchr(cmd_args[i], '=');
		if (equal_sign)
			name = ft_substr(cmd_args[i], 0, equal_sign - cmd_args[i]);
		else
			name = ft_strdup(cmd_args[i]);
		if (name && !is_right_identifier(name))
		{
			msg_export(name, cmd_args[i]);
			return (1);
		}
		if (name)
			free(name);
		i++;
	}
	return (0);
}

static int	handle_cd_in_pipeline(char **cmd_args, char **envp)
{
	char	*target_dir;

	if (cmd_args[1] && cmd_args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	if (cmd_args[1] == NULL || ft_strcmp(cmd_args[1], "~") == 0)
	{
		target_dir = get_env_value(envp, "HOME");
		if (target_dir == NULL)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		target_dir = cmd_args[1];
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	execute_other_builtins(char *cmd_name, char **cmd_args,
		t_shell_data *data)
{
	if (ft_strcmp(cmd_name, "export") == 0)
		return (handle_export_in_pipeline(cmd_args));
	else if (ft_strcmp(cmd_name, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd_name, "cd") == 0)
		return (handle_cd_in_pipeline(cmd_args, data->envp));
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		return (handle_pwd_command());
	else if (ft_strcmp(cmd_name, "env") == 0)
		return (handle_env_command(data->envp));
	else if (ft_strcmp(cmd_name, "echo") == 0)
		return (handle_echo_in_pipeline(cmd_args));
	else if (ft_strcmp(cmd_name, "neofetch") == 0)
		return (handle_neofetch());
	return (1);
}

int	execute_builtin_in_pipeline(char *cmd_name, char **cmd_args, char **envp)
{
	t_shell_data	data;

	data.envp = envp;
	data.last_exit_status = 0;
	if (ft_strcmp(cmd_name, "exit") == 0)
	{
		if (cmd_args[1])
		{
			if (!is_numeric_string(cmd_args[1]))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(cmd_args[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				return (2);
			}
			return ((unsigned char)ft_atoi(cmd_args[1]));
		}
		return (0);
	}
	return (execute_other_builtins(cmd_name, cmd_args, &data));
}
