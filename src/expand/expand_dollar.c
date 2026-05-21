/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_status(char *dst, int *di)
{
	char	*itoa;
	int		k;

	itoa = ft_itoa(*status_ref());
	if (!itoa)
		return (0);
	k = 0;
	while (itoa[k])
		dst[(*di)++] = itoa[k++];
	free(itoa);
	return (1);
}

int	var_name_len(const char *src, int start)
{
	int	len;

	len = 0;
	while (src[start + len]
		&& (ft_isalnum(src[start + len]) || src[start + len] == '_'))
		len++;
	return (len);
}

int	expand_name(const char *src, int start, int len, char *dst)
{
	char	name[MSH_NAMECAP];
	char	*val;
	int		written;
	int		k;

	k = 0;
	while (k < len && k < MSH_NAMECAP - 1)
	{
		name[k] = src[start + k];
		k++;
	}
	name[k] = '\0';
	val = env_get(name);
	written = 0;
	if (val)
	{
		k = 0;
		while (val[k])
			dst[written++] = val[k++];
	}
	return (written);
}

static int	can_start_var(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	expand_dollar(const char *src, int *si, char *dst, int *di)
{
	int	start;
	int	len;
	int	wrote;

	if (src[*si] == '$' && src[*si + 1] == '?')
	{
		*si += 2;
		return (expand_status(dst, di));
	}
	if (src[*si] == '$' && can_start_var(src[*si + 1]))
	{
		(*si)++;
		start = *si;
		len = var_name_len(src, start);
		*si += len;
		wrote = expand_name(src, start, len, dst + *di);
		*di += wrote;
		return (1);
	}
	dst[(*di)++] = src[(*si)++];
	return (0);
}
