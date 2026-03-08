/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:57:20 by riramana          #+#    #+#             */
/*   Updated: 2025/11/24 07:35:40 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	skip_quoted_segment(char *str, int *i)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i])
		(*i)++;
}

static void	skip_unquoted_segment(char *str, int *i)
{
	while (str[*i] && str[*i] != ' ' && str[*i] != '\t' && str[*i] != '\''
		&& str[*i] != '"')
		(*i)++;
}

int	count_echo_args(char *str)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		if (!str[i])
			break ;
		if (str[i] == '\'' || str[i] == '"')
			skip_quoted_segment(str, &i);
		else
			skip_unquoted_segment(str, &i);
		if (str[i])
			count++;
	}
	return (count);
}

void	close_echo_fds(t_echo_redir *redir)
{
	if (redir->heredoc_fd != -1)
		close(redir->heredoc_fd);
	if (redir->input_fd != -1)
		close(redir->input_fd);
}
