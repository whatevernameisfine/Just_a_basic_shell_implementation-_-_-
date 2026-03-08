/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils11.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:30:25 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:04:51 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*build_filename_parts(char *line, int *pos)
{
	char	*result;
	char	*temp;
	char	*join;

	result = ft_strdup("");
	while (line[*pos] && line[*pos] != ' ' && line[*pos] != '\t'
		&& line[*pos] != '>' && line[*pos] != '<' && line[*pos] != '|')
	{
		if (line[*pos] == '\'' || line[*pos] == '"')
			temp = extract_quoted_filename(line, pos);
		else
			temp = extract_unquoted_filename(line, pos);
		join = ft_strjoin(result, temp);
		free(result);
		free(temp);
		if (!join)
			return (NULL);
		result = join;
	}
	return (result);
}

int	open_input_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
	return (fd);
}

int	open_and_check_file(char *filename, int mode)
{
	int	fd;

	fd = open(filename, mode, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (-1);
	}
	close(fd);
	return (0);
}

int	external_ret(t_exec_params	*params, int syn_er)
{
	cleanup_params(params);
	if (syn_er)
		return (2);
	return (1);
}
