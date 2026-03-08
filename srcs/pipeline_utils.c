/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:43:20 by riramana          #+#    #+#             */
/*   Updated: 2025/12/16 23:15:29 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_pipes(char *command)
{
	int	count;
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	count = 0;
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (command[i])
	{
		if (command[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (command[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (command[i] == '|' && !in_single_quote && !in_double_quote)
			count++;
		i++;
	}
	return (count);
}

static int	count_pipe_segments(char *cmd, int in_sq, int in_dq)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (cmd[i])
	{
		if (cmd[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (cmd[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else if (cmd[i] == '|' && !in_sq && !in_dq)
			count++;
		i++;
	}
	return (count);
}

static char	*extract_segment(char *cmd, int *pos, int in_sq, int in_dq)
{
	int		start;
	char	*seg;

	start = *pos;
	while (cmd[*pos])
	{
		if (cmd[*pos] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (cmd[*pos] == '"' && !in_sq)
			in_dq = !in_dq;
		else if (cmd[*pos] == '|' && !in_sq && !in_dq)
			break ;
		(*pos)++;
	}
	seg = ft_substr(cmd, start, *pos - start);
	if (cmd[*pos] == '|')
		(*pos)++;
	return (seg);
}

char	**split_pipeline(char *command)
{
	char	**result;
	int		count;
	int		i;
	int		pos;

	count = count_pipe_segments(command, 0, 0);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < count)
	{
		result[i] = extract_segment(command, &pos, 0, 0);
		if (!result[i])
		{
			while (i > 0)
				free(result[--i]);
			free(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

void	free_pipeline_data(char ***cmds, char **paths, int n)
{
	int	i;

	if (cmds)
	{
		i = 0;
		while (i < n)
		{
			if (cmds[i])
				free_split(cmds[i]);
			i++;
		}
		free(cmds);
	}
	if (paths)
	{
		i = 0;
		while (i < n)
		{
			if (paths[i])
				free(paths[i]);
			i++;
		}
		free(paths);
	}
}
