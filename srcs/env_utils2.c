/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:28:28 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:48:26 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**add_env_entry(char **envp, char *new_entry)
{
	char	**new_envp;
	int		count;
	int		i;

	count = env_count(envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
	{
		free(new_entry);
		return (envp);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[i] = new_entry;
	new_envp[i + 1] = NULL;
	free(envp);
	return (new_envp);
}

static void	copy_env_except_idx(char **envp, char **new_envp, int idx, int cnt)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < cnt)
	{
		if (i != idx)
			new_envp[j++] = envp[i];
		else
			free(envp[i]);
		i++;
	}
	new_envp[j] = NULL;
}

char	**remove_env_entry(char **envp, char *name)
{
	char	**new_envp;
	int		idx;
	int		count;

	idx = find_env_index(envp, name);
	if (idx < 0)
		return (envp);
	count = env_count(envp);
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (envp);
	copy_env_except_idx(envp, new_envp, idx, count);
	free(envp);
	return (new_envp);
}
