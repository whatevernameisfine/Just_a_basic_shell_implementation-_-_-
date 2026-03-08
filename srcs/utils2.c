/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:20:00 by riramana          #+#    #+#             */
/*   Updated: 2026/03/08 11:22:06 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*trim_whitespace(char *str)
{
	char	*end;
	char	*result;
	int		len;

	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == 0)
		return (ft_strdup(""));
	end = str + ft_strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;
	len = end - str + 1;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, str, len);
	result[len] = '\0';
	return (result);
}

int	handle_fork_error(char **cmd_args, char *exec_path)
{
	perror("fork failed");
	free_split(cmd_args);
	free(exec_path);
	return (-1);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, ".") == 0)
		return (1);
	if (ft_strcmp(cmd, "minifetch") == 0)
		return (1);
	return (0);
}

void	swap_env_entries(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	check_overflow(char *str, int sign)
{
	unsigned long long	result;
	int					i;
	int					digit;

	result = 0;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (sign == 1 && result > (9223372036854775807ULL - digit) / 10)
			return (1);
		if (sign == -1 && result > (9223372036854775808ULL - digit) / 10)
			return (1);
		result = result * 10 + digit;
		i++;
	}
	return (0);
}
