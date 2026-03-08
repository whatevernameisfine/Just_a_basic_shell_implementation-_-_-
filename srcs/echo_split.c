/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:57:32 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:23:09 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_quoted(char *str, int *pos, t_shell_data *data)
{
	char	quote;
	int		start;
	int		i;
	char	*content;
	char	*expanded;

	quote = str[*pos];
	start = *pos + 1;
	i = start;
	while (str[i] && str[i] != quote)
		i++;
	*pos = i + 1;
	content = ft_substr(str, start, i - start);
	if (quote == '"' && data)
	{
		expanded = expand_string(content, data);
		free(content);
		return (expanded);
	}
	return (content);
}

char	*extract_unquoted(char *str, int *pos, t_shell_data *data)
{
	int		start;
	int		i;
	char	*content;
	char	*expanded;

	start = *pos;
	i = start;
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\''
		&& str[i] != '"')
		i++;
	*pos = i;
	content = ft_substr(str, start, i - start);
	if (data)
	{
		expanded = expand_string(content, data);
		free(content);
		return (expanded);
	}
	return (content);
}

char	*extract_arg(char *str, int *pos)
{
	return (extract_arg_with_data(str, pos, NULL));
}

char	*extract_arg_with_data(char *str, int *pos, t_shell_data *data)
{
	char	*result;
	char	*temp;
	char	*join;

	result = ft_strdup("");
	while (str[*pos] && str[*pos] != ' ' && str[*pos] != '\t')
	{
		if (str[*pos] == '\'' || str[*pos] == '"')
			temp = extract_quoted(str, pos, data);
		else
			temp = extract_unquoted(str, pos, data);
		join = ft_strjoin(result, temp);
		if (!join)
		{
			free(result);
			free(temp);
			return (NULL);
		}
		free(result);
		free(temp);
		result = join;
	}
	return (result);
}

char	**split_echo_args(char *command)
{
	char	**args;
	int		arg_count;

	arg_count = count_echo_args(command);
	args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	if (!parse_args(command, args))
		return (NULL);
	return (args);
}
