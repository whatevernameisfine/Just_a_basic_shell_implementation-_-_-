/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:57:41 by riramana          #+#    #+#             */
/*   Updated: 2026/01/06 10:58:15 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	has_unclosed_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
				return (quote);
			i++;
		}
		else
			i++;
	}
	return (0);
}

int	check_quotes(char *command)
{
	int	unclosed;

	unclosed = has_unclosed_quotes(command);
	if (unclosed)
	{
		ft_putstr_fd("minishell: syntax error: quotes not closed\n", 2);
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
		return (1);
	}
	return (0);
}

static void	free_args_on_error(char **args, int count)
{
	while (--count >= 0)
		free(args[count]);
	free(args);
}

int	parse_args(char *command, char **args)
{
	int	i;
	int	pos;

	i = 0;
	pos = 0;
	while (command[pos])
	{
		while (command[pos] && (command[pos] == ' ' || command[pos] == '\t'))
			pos++;
		if (!command[pos])
			break ;
		args[i] = extract_arg(command, &pos);
		if (!args[i])
		{
			free_args_on_error(args, i);
			return (0);
		}
		i++;
	}
	args[i] = NULL;
	return (1);
}

int	parse_args_with_data(char *command, char **args, t_shell_data *data)
{
	int	i;
	int	pos;
	int	had_quotes;

	init_two(&i, &pos);
	while (command[pos])
	{
		while (command[pos] && (command[pos] == ' ' || command[pos] == '\t'))
			pos++;
		had_quotes = (command[pos] == '\'' || command[pos] == '"');
		args[i] = extract_arg_with_data(command, &pos, data);
		if (!args[i])
		{
			free_args_on_error(args, i);
			return (0);
		}
		if (args[i][0] == '\0' && !had_quotes)
		{
			free(args[i]);
			continue ;
		}
		i++;
	}
	args[i] = NULL;
	return (1);
}
