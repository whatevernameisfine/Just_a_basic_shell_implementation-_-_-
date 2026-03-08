/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:58:19 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:33:19 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		return (1);
	return (0);
}

static char	*build_path(char *dir, char *cmd_name)
{
	char	*temp_path;
	char	*full_path;
	char	*dup;

	dup = ft_strdup(dir);
	temp_path = ft_strjoin(dup, "/");
	full_path = ft_strjoin(temp_path, cmd_name);
	free(temp_path);
	free(dup);
	return (full_path);
}

static char	*check_path_access(char **path_dirs, char *cmd_name)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = build_path(path_dirs[i], cmd_name);
		if (access(full_path, X_OK) == 0 && !is_directory(full_path))
		{
			free_split(path_dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable_path(char *cmd_name, char **envp)
{
	char	**path_dirs;
	char	*path_env;
	char	*result;

	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, F_OK) == 0)
		{
			if (access(cmd_name, X_OK) == 0 && !is_directory(cmd_name))
				return (ft_strdup(cmd_name));
		}
		return (NULL);
	}
	path_env = get_env_value(envp, "PATH");
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	result = check_path_access(path_dirs, cmd_name);
	if (!result)
		free_split(path_dirs);
	return (result);
}

int	handle_dot_builtin(char **cmd_args)
{
	if (!cmd_args[1])
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		return (2);
	}
	ft_putstr_fd("minishell: .: ", 2);
	ft_putstr_fd(cmd_args[1], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (1);
}
