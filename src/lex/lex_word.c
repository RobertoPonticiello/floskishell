/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	word_end(char c)
{
	return (c == '\0' || c == ' ' || c == '\t'
		|| c == '|' || c == '<' || c == '>');
}

static void	skip_to_close(const char *src, int *idx)
{
	char	quote;

	quote = src[*idx];
	(*idx)++;
	while (src[*idx] && src[*idx] != quote)
	{
		if (quote == '"' && src[*idx] == '\\' && src[*idx + 1])
			(*idx) += 2;
		else
			(*idx)++;
	}
	if (src[*idx] == quote)
		(*idx)++;
}

static void	advance_double_escape(const char *src, int *idx, int *len)
{
	if (src[*idx] == '\\' && src[*idx + 1])
	{
		(*idx) += 2;
		*len += 2;
	}
	else
	{
		(*idx)++;
		(*len)++;
	}
}

char	*lex_quoted_word(const char *src, int *idx, char q)
{
	int		start;
	int		len;

	start = *idx + 1;
	len = 0;
	(*idx)++;
	while (src[*idx] && src[*idx] != q)
	{
		if (q == '"')
			advance_double_escape(src, idx, &len);
		else
		{
			(*idx)++;
			len++;
		}
	}
	if (src[*idx] == q)
		(*idx)++;
	return (ft_substr(src, start, len));
}

char	*lex_bare_word(const char *src, int *idx)
{
	int	start;

	start = *idx;
	while (src[*idx] && !word_end(src[*idx]))
	{
		if (src[*idx] == '\'' || src[*idx] == '"')
			skip_to_close(src, idx);
		else
			(*idx)++;
	}
	return (ft_substr(src, start, *idx - start));
}
