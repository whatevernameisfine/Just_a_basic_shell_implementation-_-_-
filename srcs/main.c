/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 01:52:55 by riramana          #+#    #+#             */
/*   Updated: 2026/01/04 12:44:23 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			g_signal = 0;

static int	validate_and_prepare_line(char *line, char **trimmed,
		t_shell_data *data)
{
	if (!line || !*line || is_whitespace(line))
		return (0);
	if (check_quotes(line))
	{
		data->last_exit_status = 2;
		return (0);
	}
	add_history(line);
	*trimmed = trim_whitespace(line);
	if (!*trimmed || !**trimmed)
	{
		if (*trimmed)
			free(*trimmed);
		return (0);
	}
	return (1);
}

static void	process_input_line(char *line, t_shell_data *data)
{
	char	*trimmed;

	if (!validate_and_prepare_line(line, &trimmed, data))
		return ;
	data->current_cmd = trimmed;
	signal(SIGINT, signal_exec);
	data->last_exit_status = execute_command(trimmed, data);
	if (g_signal == SIGINT)
	{
		data->last_exit_status = 130;
		g_signal = 0;
	}
	signal(SIGINT, signal_thing);
	data->current_cmd = NULL;
	free(trimmed);
}

static int	handle_readline_result(char *line, t_shell_data *data)
{
	if (!line)
	{
		write(1, "exit\n", 5);
		free_split(data->envp);
		exit(0);
	}
	return (0);
}

static void	process_line_loop(t_shell_data *data)
{
	char	*line;

	while (1)
	{
		line = readline(PROMPT " ");
		if (g_signal == SIGINT)
		{
			data->last_exit_status = 130;
			g_signal = 0;
		}
		if (handle_readline_result(line, data))
			break ;
		process_input_line(line, data);
		if (line)
			free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	data;

	(void)argc;
	if (argv[1])
	{
		printf("Minishell takes no arguments\n");
		exit(0);
	}
	data.envp = copy_env(envp);
	if (!data.envp)
		return (1);
	data.last_exit_status = 0;
	data.current_cmd = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_thing);
	process_line_loop(&data);
	free_split(data.envp);
	return (0);
}
