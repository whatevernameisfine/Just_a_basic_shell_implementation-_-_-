/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:44:38 by riramana          #+#    #+#             */
/*   Updated: 2025/11/24 18:10:11 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*process_double_quotes(char *str, int *i, t_shell_data *data)
{
	int		start;
	int		len;
	char	*content;
	char	*expanded;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '"')
		(*i)++;
	len = *i - start;
	if (str[*i] == '"')
		(*i)++;
	content = ft_substr(str, start, len);
	if (!content)
		return (ft_strdup(""));
	expanded = expand_string(content, data);
	free(content);
	if (!expanded)
		return (ft_strdup(""));
	return (expanded);
}

static char	*process_single_quotes(char *str, int *i)
{
	int		start;
	int		len;
	char	*content;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	len = *i - start;
	if (str[*i] == '\'')
		(*i)++;
	content = ft_substr(str, start, len);
	if (!content)
		return (ft_strdup(""));
	return (content);
}

static char	*process_unquoted(char *str, int *i, t_shell_data *data)
{
	int		start;
	int		len;
	char	*content;
	char	*expanded;

	start = *i;
	while (str[*i] && str[*i] != '"' && str[*i] != '\'' && str[*i] != ' '
		&& str[*i] != '\t')
		(*i)++;
	len = *i - start;
	content = ft_substr(str, start, len);
	if (!content)
		return (ft_strdup(""));
	expanded = expand_string(content, data);
	free(content);
	if (!expanded)
		return (ft_strdup(""));
	return (expanded);
}

char	*expand_command_line(char *line, t_shell_data *data)
{
	char	*result;
	char	*part;
	char	*temp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '"')
			part = process_double_quotes(line, &i, data);
		else if (line[i] == '\'')
			part = process_single_quotes(line, &i);
		else if (line[i] == ' ' || line[i] == '\t')
			part = ft_substr(line, i++, 1);
		else
			part = process_unquoted(line, &i, data);
		temp = ft_strjoin(result, part);
		free(result);
		free(part);
		result = temp;
	}
	return (result);
}
