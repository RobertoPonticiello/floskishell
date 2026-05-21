/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*tok_new(const char *raw, t_tktype kind)
{
	t_tok	*node;

	node = (t_tok *)malloc(sizeof(t_tok));
	if (!node)
		return (NULL);
	node->raw = ft_strdup(raw);
	node->kind = kind;
	node->quoted = 0;
	node->nxt = NULL;
	return (node);
}

int	is_op_start(const char *src, int i)
{
	return (src[i] == '|' || src[i] == '<' || src[i] == '>');
}

static t_tok	*compound_op(const char *src, int *idx)
{
	if (src[*idx] == '<' && src[*idx + 1] == '<')
	{
		*idx += 2;
		return (tok_new("<<", TK_HDOC));
	}
	if (src[*idx] == '>' && src[*idx + 1] == '>')
	{
		*idx += 2;
		return (tok_new(">>", TK_APPEND));
	}
	return (NULL);
}

static t_tok	*simple_op(const char *src, int *idx)
{
	char	c;

	c = src[*idx];
	(*idx)++;
	if (c == '<')
		return (tok_new("<", TK_IN));
	if (c == '>')
		return (tok_new(">", TK_OUT));
	return (tok_new("|", TK_PIPE));
}

t_tok	*lex_op(const char *src, int *idx)
{
	t_tok	*node;

	node = compound_op(src, idx);
	if (node)
		return (node);
	return (simple_op(src, idx));
}
