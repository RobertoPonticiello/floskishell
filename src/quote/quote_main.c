/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	mark_quoted(t_tok *tk)
{
	if (ft_strchr(tk->raw, '\'') || ft_strchr(tk->raw, '"'))
		tk->quoted = 1;
}

static void	strip_one(t_tok *tk)
{
	char	*clean;

	if (!tk || !tk->raw || tk->kind != TK_WORD)
		return ;
	mark_quoted(tk);
	clean = quote_strip_str(tk->raw);
	if (!clean)
		return ;
	free(tk->raw);
	tk->raw = clean;
}

void	quote_strip_list(t_tok *list)
{
	t_tok	*cur;

	cur = list;
	while (cur)
	{
		if (cur->kind == TK_WORD)
			strip_one(cur);
		cur = cur->nxt;
	}
}

char	*quote_strip_str(char *src)
{
	char	*buf;
	int		i;
	int		j;

	buf = malloc(MSH_EXPBUF);
	if (!buf)
		return (NULL);
	i = 0;
	j = 0;
	while (src[i])
		qs_step(src, &i, buf, &j);
	buf[j] = '\0';
	return (buf);
}
