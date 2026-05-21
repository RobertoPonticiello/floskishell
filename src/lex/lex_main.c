/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	tok_append(t_tok **head, t_tok **tail, t_tok *node)
{
	if (!node)
		return ;
	if (!*head)
		*head = node;
	else
		(*tail)->nxt = node;
	*tail = node;
}

static void	emit_word(const char *src, int *idx, t_tok **h, t_tok **t)
{
	char	*piece;

	piece = lex_bare_word(src, idx);
	if (!piece)
		return ;
	if (ft_strlen(piece) > 0)
		tok_append(h, t, tok_new(piece, TK_WORD));
	free(piece);
}

static void	skip_blanks(const char *src, int *idx)
{
	while (src[*idx] == ' ' || src[*idx] == '\t')
		(*idx)++;
}

static void	step(const char *src, int *idx, t_tok **h, t_tok **t)
{
	if (is_op_start(src, *idx))
		tok_append(h, t, lex_op(src, idx));
	else
		emit_word(src, idx, h, t);
}

t_tok	*lex_line(const char *src)
{
	t_tok	*head;
	t_tok	*tail;
	int		i;

	if (!src)
		return (NULL);
	head = NULL;
	tail = NULL;
	i = 0;
	while (src[i])
	{
		skip_blanks(src, &i);
		if (!src[i])
			break ;
		step(src, &i, &head, &tail);
	}
	return (head);
}
