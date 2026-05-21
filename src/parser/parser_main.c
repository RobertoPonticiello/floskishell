/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse(const char *line)
{
	t_tok	*toks;
	t_cmd	*cmds;

	if (!line || !*line)
		return (NULL);
	toks = lex_line(line);
	if (!toks)
		return (NULL);
	expand_list(&toks);
	quote_strip_list(toks);
	if (!syntax_ok(toks))
	{
		tok_free(toks);
		return (NULL);
	}
	cmds = build_cmds(toks);
	tok_free(toks);
	return (cmds);
}
