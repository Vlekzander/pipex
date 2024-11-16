/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 11:20:01 by apierret          #+#    #+#             */
/*   Updated: 2024/11/16 18:56:43 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "utils.h"

#include <libft.h>

static size_t	ft_countargs(char const *s)
{
	size_t	count;
	int		in_word;
	char	g_char;

	count = 0;
	in_word = 0;
	g_char = 0;
	while (*s != '\0')
	{
		if (!in_word && (*s != ' ' || *s == '"' || *s == '\''))
		{
			if (*s == '"' || *s == '\'')
				g_char = *s;
			count += g_char == 0;
			in_word = 1;
		}
		else if (in_word && ((!g_char && *s == ' ') || g_char == *s))
		{
			count += g_char != 0;
			in_word = 0;
			g_char = 0;
		}
		s++;
	}
	return (count);
}

static char	*ft_substr_exc(char const *s, unsigned int start,
	size_t len, char *exclude_set)
{
	char	*str;
	size_t	str_len;
	size_t	i;

	if (s == NULL)
		return (NULL);
	str_len = ft_strlen(s);
	if (str_len < start)
		str_len = 0;
	else
		str_len -= start;
	if (str_len > len)
		str_len = len;
	str = (char *) ft_calloc(str_len +1, sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < str_len && s[i + start] != '\0')
	{
		if (ft_strchr(exclude_set, s[i + start]) == NULL)
			str[i] = s[i + start];
		i++;
	}
	return (str);
}

static char	*allocate_args(const char *s, size_t *i)
{
	size_t	start;
	size_t	len;
	int		in_group;
	char	g_char;

	in_group = 0;
	g_char = 0;
	while (s[*i] != '\0' && s[*i] == ' ')
		(*i)++;
	while (s[*i] != '\0' && (s[*i] == '"' || s[*i] == '\''))
	{
		in_group = !in_group;
		if (in_group)
			g_char = s[*i];
		(*i)++;
	}
	start = *i;
	while (s[*i] != '\0' && (in_group && s[*i] != g_char))
		(*i)++;
	while (s[*i] != '\0' && s[*i] != ' ')
		(*i)++;
	len = *i - start - (g_char != 0);
	return (ft_substr_exc(s, start, len, "\"'"));
}

void	free_ddarray(char **arr)
{
	size_t	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_split_args(char *cmd)
{
	char	**args;
	size_t	i;
	size_t	j;

	i = ft_countargs(cmd);
	args = (char **) ft_calloc(i +1, sizeof(char *));
	if (args == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] != ' ' || cmd[i] == '"' || cmd[i] == '\'')
		{
			args[j] = allocate_args(cmd, &i);
			if (args[j] == NULL)
				return (free_ddarray(args), NULL);
			j++;
		}
		else
			i++;
	}
	return (args);
}
