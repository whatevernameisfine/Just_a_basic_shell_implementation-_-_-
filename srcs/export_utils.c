/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:31:38 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:23:10 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_right_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_sorted_env(char **envp)
{
	char	**sorted;
	int		i;
	int		j;

	sorted = copy_env(envp);
	if (!sorted)
		return ;
	i = 0;
	while (sorted[i])
	{
		j = i + 1;
		while (sorted[j])
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
				swap_env_entries(&sorted[i], &sorted[j]);
			j++;
		}
		printf("declare -x %s\n", sorted[i]);
		i++;
	}
	free_split(sorted);
}
