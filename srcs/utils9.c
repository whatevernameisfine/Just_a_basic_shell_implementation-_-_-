/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils9.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 11:12:38 by andyandr          #+#    #+#             */
/*   Updated: 2026/01/08 14:05:59 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	msg_exit_builtin(char *str)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	init_stru_data(t_pipeline_data *data, int i)
{
	data->cmds[i] = NULL;
	data->paths[i] = NULL;
}

void	free_stru(t_exec_data *data)
{
	free(data->cmds);
	free(data->paths);
	free(data->out_files);
	free(data->out_modes);
	free(data->heredoc_fds);
	free(data->input_fds);
}

void	assignment_result(t_pipeline_data *p_data, t_pipeline_redir *redir)
{
	redir->out_files = p_data->out_files;
	redir->out_modes = p_data->out_modes;
	redir->heredoc_fds = p_data->heredoc_fds;
	redir->input_fds = p_data->input_fds;
	redir->n = p_data->n;
}

void	init_echo_redir(t_echo_redir *redir)
{
	redir->heredoc_fd = -1;
	redir->input_fd = -1;
	redir->out_file = NULL;
	redir->out_mode = -1;
}
