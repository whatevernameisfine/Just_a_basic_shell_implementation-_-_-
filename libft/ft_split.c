/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riramana <riramana@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:26:47 by riramana          #+#    #+#             */
/*   Updated: 2025/04/06 13:41:15 by riramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free_array(char **array, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static	size_t	ft_count_words(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static	size_t	ft_word_length(char const *s, char c, size_t start)
{
	size_t	len;

	len = 0;
	while (s[start + len] && s[start + len] != c)
		len++;
	return (len);
}

static	char	**ft_create_array(char const *s, char c, size_t word_count)
{
	char	**array;
	size_t	i;
	size_t	j;
	size_t	word_len;

	array = malloc(sizeof(char *) * (word_count + 1));
	if (!array)
		return (NULL);
	i = 0;
	j = 0;
	while (j < word_count)
	{
		while (s[i] == c)
			i++;
		word_len = ft_word_length(s, c, i);
		array[j] = ft_substr(s, i, word_len);
		if (!array[j])
			return (ft_free_array(array, j), NULL);
		i += word_len;
		j++;
	}
	array[j] = NULL;
	return (array);
}

char	**ft_split(const char *s, char c)
{
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = ft_count_words(s, c);
	return (ft_create_array(s, c, word_count));
}
