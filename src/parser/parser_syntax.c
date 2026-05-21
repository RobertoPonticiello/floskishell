/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir(t_tktype k)
{
	return (k == TK_IN || k == TK_OUT || k == TK_HDOC || k == TK_APPEND);
}

static int	check_pipe(t_tok *cur)
{
	if (!cur->nxt || cur->nxt->kind == TK_PIPE)
		return (0);
	return (1);
}

static int	check_redir(t_tok *cur)
{
	if (!cur->nxt || cur->nxt->kind != TK_WORD)
		return (0);
	return (1);
}

int	syntax_ok(t_tok *toks)
{
	t_tok	*cur;

	if (!toks)
		return (1);
	if (toks->kind == TK_PIPE)
		return (0);
	cur = toks;
	while (cur)
	{
		if (cur->kind == TK_PIPE && !check_pipe(cur))
			return (0);
		if (is_redir(cur->kind) && !check_redir(cur))
			return (0);
		cur = cur->nxt;
	}
	return (1);
}
