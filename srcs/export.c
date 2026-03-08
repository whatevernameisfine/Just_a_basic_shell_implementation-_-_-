/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:01:48 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:27:53 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_export_error(char *arg)
{
	char	*msg;
	char	*temp;

	temp = ft_strjoin("minishell: export: `", arg);
	if (!temp)
		return ;
	msg = ft_strjoin(temp, "': not a valid identifier\n");
	free(temp);
	if (!msg)
		return ;
	write(2, msg, ft_strlen(msg));
	free(msg);
}

static int	validate_and_export(t_shell_data *data, char *name, char *value)
{
	if (!is_right_identifier(name))
	{
		print_export_error(name);
		free(name);
		return (1);
	}
	data->envp = set_env_value(data->envp, name, value);
	free(name);
	return (0);
}

static int	export_variable(t_shell_data *data, char *arg)
{
	char	*equal_sign;
	char	*name;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		if (!is_right_identifier(arg))
		{
			print_export_error(arg);
			return (1);
		}
		return (0);
	}
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!name)
		return (1);
	return (validate_and_export(data, name, equal_sign + 1));
}

static int	process_export_args(t_shell_data *data, char **cmd_args)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (cmd_args[i])
	{
		if (export_variable(data, cmd_args[i]))
			ret = 1;
		i++;
	}
	return (ret);
}

int	handle_export_command(t_shell_data *data, char **cmd_args)
{
	if (!cmd_args[1])
	{
		print_sorted_env(data->envp);
		return (0);
	}
	return (process_export_args(data, cmd_args));
}
