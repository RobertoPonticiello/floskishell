/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_escape.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_literal_pass(char c)
{
	return (c == '"' || c == '\\' || c == '$');
}

int	esc_apply(const char *s, int *i, char *dst, int *j)
{
	char	c;

	(*i)++;
	c = s[*i];
	if (!c)
		return (0);
	if (is_literal_pass(c))
		dst[(*j)++] = c;
	else if (c == 'n')
		dst[(*j)++] = '\n';
	else if (c == 't')
		dst[(*j)++] = '\t';
	else
	{
		dst[(*j)++] = '\\';
		dst[(*j)++] = c;
	}
	(*i)++;
	return (1);
}
