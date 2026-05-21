/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build.c                                     :+:      :+:    :+:   */
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

static t_tok	*consume_redir(t_cmd *cmd, t_tok *cur)
{
	int	was_hdoc;

	was_hdoc = (cur->kind == TK_HDOC);
	if (apply_redir(cmd, cur) == -1)
	{
		cmd->redir_err = 1;
		return (NULL);
	}
	if (was_hdoc)
		return (skip_hdoc_cluster(cur));
	cur = cur->nxt;
	if (cur)
		cur = cur->nxt;
	return (cur);
}

static t_tok	*scan_segment(t_tok *cur, int *n, char **argv, t_cmd *cmd)
{
	*n = 0;
	while (cur && cur->kind != TK_PIPE)
	{
		if (cur->kind == TK_WORD)
		{
			argv[(*n)++] = ft_strdup(cur->raw);
			cur = cur->nxt;
		}
		else if (is_redir(cur->kind))
		{
			cur = consume_redir(cmd, cur);
			if (!cur)
				break ;
		}
		else
			cur = cur->nxt;
	}
	return (cur);
}

static t_cmd	*build_one(t_tok *cur)
{
	t_cmd	*cmd;
	char	*argv[MSH_NAMECAP];
	int		n;

	cmd = cmd_new();
	if (!cmd)
		return (NULL);
	scan_segment(cur, &n, argv, cmd);
	argv[n] = NULL;
	return (cmd_finalize(cmd, n, argv));
}

t_cmd	*build_cmds(t_tok *toks)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*node;
	t_tok	*cur;

	head = NULL;
	tail = NULL;
	cur = toks;
	while (cur)
	{
		node = build_one(cur);
		if (node)
			cmd_push(&head, &tail, node);
		cur = cmd_skip_to_next(cur);
	}
	return (head);
}
