/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_walk.c                                      :+:      :+:    :+:   */
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

t_tok	*cmd_skip_to_next(t_tok *tk)
{
	while (tk && tk->kind != TK_PIPE)
	{
		if (is_redir(tk->kind))
		{
			tk = tk->nxt;
			if (tk)
				tk = tk->nxt;
		}
		else
			tk = tk->nxt;
	}
	if (tk && tk->kind == TK_PIPE)
		tk = tk->nxt;
	return (tk);
}

t_tok	*skip_hdoc_cluster(t_tok *cur)
{
	while (cur && cur->kind == TK_HDOC)
	{
		cur = cur->nxt;
		if (cur)
			cur = cur->nxt;
	}
	return (cur);
}
