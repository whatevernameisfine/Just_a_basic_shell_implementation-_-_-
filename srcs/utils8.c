/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 11:02:22 by andyandr          #+#    #+#             */
/*   Updated: 2026/01/08 13:46:08 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	msg_export(char *name, char *cmd)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(name);
}

int	init_two(int *arg1, int *arg2)
{
	*arg1 = 0;
	*arg2 = 0;
	return (0);
}

void	init_exec_param(t_exec_params *params)
{
	params->out_file = NULL;
	params->out_mode = -1;
	params->heredoc_fd = -1;
	params->input_fd = -1;
}

int	is_out_or_in(char *command, t_exec_params *params, int *syntax_err,
	char **clns)
{
	int		err1;
	int		err2;
	char	*clnf;

	init_two(&err1, &err2);
	clnf = parse_output_redir_with_err(command, &params->out_file,
			&params->out_mode, &err1);
	if (!clnf)
	{
		*syntax_err = err1;
		return (0);
	}
	*clns = parse_input_redir_with_err(clnf,
			&params->heredoc_fd, &params->input_fd, &err2);
	free(clnf);
	*syntax_err = err2;
	return (1);
}

int	is_in_or_out(char *command, t_exec_params *params, int *syntax_err
	, char **clns)
{
	int		err1;
	int		err2;
	char	*clnf;

	init_two(&err1, &err2);
	clnf = parse_input_redir_with_err(command, &params->heredoc_fd,
			&params->input_fd, &err1);
	if (!clnf)
	{
		*syntax_err = err1;
		return (0);
	}
	*clns = parse_output_redir_with_err(clnf,
			&params->out_file, &params->out_mode, &err2);
	free(clnf);
	*syntax_err = err2;
	return (1);
}
