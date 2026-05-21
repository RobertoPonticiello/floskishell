/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_hdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hdoc_count(t_tok *tk)
{
	t_tok	*cur;
	int		n;

	cur = tk;
	n = 0;
	while (cur && cur->kind != TK_PIPE)
	{
		if (cur->kind == TK_HDOC)
			n++;
		cur = cur->nxt;
	}
	return (n);
}

static char	*assemble_name(char *idx, char *pid)
{
	char	*base;
	char	*mid;
	char	*final;

	base = ft_strjoin("/tmp/.msh_hdoc_", idx);
	if (!base)
		return (NULL);
	mid = ft_strjoin(base, "_");
	free(base);
	if (!mid)
		return (NULL);
	final = ft_strjoin(mid, pid);
	free(mid);
	return (final);
}

char	*hdoc_tmp_name(void)
{
	static int	counter = 0;
	char		*idx;
	char		*pid;
	char		*name;

	idx = ft_itoa(counter++);
	pid = ft_itoa(getpid());
	if (!idx || !pid)
	{
		free(idx);
		free(pid);
		return (NULL);
	}
	name = assemble_name(idx, pid);
	free(idx);
	free(pid);
	return (name);
}
