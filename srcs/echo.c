/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:57:06 by riramana          #+#    #+#             */
/*   Updated: 2026/01/07 14:27:33 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	count_options(char **cmd_args)
{
	int	i;

	i = 1;
	while (cmd_args[i] && is_valid_n_option(cmd_args[i]))
		i++;
	return (i);
}

static void	print_echo_args(char **args, int start)
{
	int	i;

	i = start;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
}

static char	*get_args_start(char *command)
{
	int	i;

	i = 0;
	while (command[i] && (command[i] == ' ' || command[i] == '\t'))
		i++;
	while (command[i] && command[i] != ' ' && command[i] != '\t')
		i++;
	return (command + i);
}

int	handle_echo_command(char *command, t_shell_data *data)
{
	char	**args;
	int		newline;
	int		start;

	if (check_quotes(get_args_start(command)))
		return (2);
	args = split_echo_args_with_data(command, data);
	if (!args)
		return (-1);
	newline = 1;
	start = count_options(args);
	if (start > 1)
		newline = 0;
	print_echo_args(args, start);
	if (newline)
		ft_putchar_fd('\n', 1);
	free_split(args);
	return (0);
}
