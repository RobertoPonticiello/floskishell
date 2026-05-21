/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pop_node(t_tok **head, t_tok *prev, t_tok **cur)
{
	t_tok	*dead;

	dead = *cur;
	if (prev)
		prev->nxt = (*cur)->nxt;
	else
		*head = (*cur)->nxt;
	*cur = (*cur)->nxt;
	free(dead->raw);
	free(dead);
}

static void	expand_node(t_tok *tk)
{
	char	*old;

	old = tk->raw;
	tk->raw = expand_str(tk->raw);
	if (old)
		free(old);
}

void	expand_list(t_tok **head)
{
	t_tok	*cur;
	t_tok	*prev;

	cur = *head;
	prev = NULL;
	while (cur)
	{
		if (cur->kind == TK_WORD)
		{
			expand_node(cur);
			if (cur->raw && ft_strlen(cur->raw) == 0)
			{
				pop_node(head, prev, &cur);
				continue ;
			}
		}
		prev = cur;
		cur = cur->nxt;
	}
}

static void	expand_loop(const char *src, char *dst, int *di)
{
	int		si;
	char	guard;

	si = 0;
	guard = 0;
	while (src[si])
	{
		if ((src[si] == '\'' || src[si] == '"') && !guard)
			guard = src[si];
		else if (src[si] == guard)
			guard = 0;
		if (guard == '\'')
		{
			dst[(*di)++] = src[si];
			si++;
		}
		else
			expand_dollar(src, &si, dst, di);
	}
}

char	*expand_str(const char *src)
{
	char	*out;
	int		di;

	if (!src)
		return (NULL);
	out = malloc(MSH_EXPBUF);
	if (!out)
		return (NULL);
	di = 0;
	expand_loop(src, out, &di);
	out[di] = '\0';
	return (out);
}
