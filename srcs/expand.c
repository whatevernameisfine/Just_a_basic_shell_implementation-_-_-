/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:48:25 by riramana          #+#    #+#             */
/*   Updated: 2026/01/11 15:48:43 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	if (ft_isdigit(str[*i]))
		(*i)++;
	else
	{
		while (str[*i] && is_valid_var_char(str[*i]))
			(*i)++;
	}
	len = *i - start;
	if (len == 0)
		return (NULL);
	name = ft_substr(str, start, len);
	return (name);
}

static char	*expand_exit_status(char *result, int exit_status)
{
	char	*status_str;
	char	*temp;

	status_str = ft_itoa(exit_status);
	if (!status_str)
		return (result);
	temp = ft_strjoin(result, status_str);
	free(status_str);
	free(result);
	return (temp);
}

static char	*expand_variable(char *result, char *str, int *i,
		t_shell_data *data)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (expand_exit_status(result, data->last_exit_status));
	}
	var_name = get_var_name(str, i);
	if (!var_name)
		return (result);
	var_value = get_env_value(data->envp, var_name);
	free(var_name);
	if (!var_value)
		return (result);
	temp = ft_strjoin(result, var_value);
	free(result);
	return (temp);
}

static char	*append_char(char *result, char c)
{
	char	*temp;
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	temp = ft_strjoin(result, buf);
	free(result);
	return (temp);
}

char	*expand_string(char *str, t_shell_data *data)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (str[i + 1] == '?'
				|| is_valid_var_char(str[i + 1])))
			result = expand_variable(result, str, &i, data);
		else
			result = append_char(result, str[i++]);
		if (!result)
			return (NULL);
	}
	return (result);
}
