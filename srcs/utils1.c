/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 01:22:59 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:34:16 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_split(char **split)
{
	char	**temp;

	if (!split)
		return ;
	temp = split;
	while (*temp)
	{
		free(*temp);
		temp++;
	}
	free(split);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

static int	check_path_error(char *cmd, struct stat *path_stat)
{
	if (stat(cmd, path_stat) == 0)
	{
		if (S_ISDIR(path_stat->st_mode))
		{
			ft_putstr_fd(": Is a directory\n", 2);
			return (126);
		}
		else if (access(cmd, X_OK) == -1)
		{
			ft_putstr_fd(": Permission denied\n", 2);
			return (126);
		}
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	ft_putstr_fd(": No such file or directory\n", 2);
	return (127);
}

int	handle_command_not_found(char **cmd_args)
{
	struct stat	path_stat;
	int			exit_code;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_args[0], 2);
	if (ft_strchr(cmd_args[0], '/'))
		exit_code = check_path_error(cmd_args[0], &path_stat);
	else
	{
		ft_putstr_fd(": command not found\n", 2);
		exit_code = 127;
	}
	free_split(cmd_args);
	return (exit_code);
}

int	is_whitespace(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}
