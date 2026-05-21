/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_step.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	qs_single(char *src, int *i, char *dst, int *j)
{
	(*i)++;
	while (src[*i] && src[*i] != '\'')
	{
		dst[*j] = src[*i];
		(*j)++;
		(*i)++;
	}
	if (src[*i] == '\'')
		(*i)++;
}

void	qs_double(char *src, int *i, char *dst, int *j)
{
	(*i)++;
	while (src[*i] && src[*i] != '"')
	{
		if (src[*i] == '\\' && src[*i + 1])
			esc_apply(src, i, dst, j);
		else if (src[*i] == '$' && src[*i + 1])
			expand_dollar(src, i, dst, j);
		else
		{
			dst[*j] = src[*i];
			(*j)++;
			(*i)++;
		}
	}
	if (src[*i] == '"')
		(*i)++;
}

void	qs_step(char *src, int *i, char *dst, int *j)
{
	if (src[*i] == '\'')
		qs_single(src, i, dst, j);
	else if (src[*i] == '"')
		qs_double(src, i, dst, j);
	else if (src[*i] == '$' && src[*i + 1])
		expand_dollar(src, i, dst, j);
	else
	{
		dst[*j] = src[*i];
		(*j)++;
		(*i)++;
	}
}
