/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:24:52 by riramana          #+#    #+#             */
/*   Updated: 2025/11/24 07:34:31 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_unset_error(char *arg)
{
	char	*msg;
	char	*temp;

	temp = ft_strjoin("minishell: unset: `", arg);
	if (!temp)
		return ;
	msg = ft_strjoin(temp, "': not a valid identifier\n");
	free(temp);
	if (!msg)
		return ;
	write(2, msg, ft_strlen(msg));
	free(msg);
}

int	handle_unset_command(t_shell_data *data, char **cmd_args)
{
	int	i;
	int	ret;

	if (!cmd_args[1])
		return (0);
	ret = 0;
	i = 1;
	while (cmd_args[i])
	{
		if (!is_valid_identifier(cmd_args[i]))
		{
			print_unset_error(cmd_args[i]);
			ret = 1;
		}
		else
			data->envp = remove_env_entry(data->envp, cmd_args[i]);
		i++;
	}
	return (ret);
}
