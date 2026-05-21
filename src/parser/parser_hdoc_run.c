/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_hdoc_run.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hdoc_one(t_cmd *cmd, t_tok *tk)
{
	int	fd;

	fd = -1;
	if (hdoc_stage(tk->nxt->raw, tk->nxt->quoted, &fd) == -1 || fd == -1)
		return (-1);
	hdoc_swap_fd(cmd, fd);
	return (0);
}

int	hdoc_many(t_cmd *cmd, t_tok *tk)
{
	char	*delims[MSH_NAMECAP];
	int		n;
	int		i;
	int		fd;
	int		ret;

	n = hdoc_count(tk);
	if (n <= 1)
		return (hdoc_one(cmd, tk));
	hdoc_collect(tk, delims, n);
	i = -1;
	ret = 0;
	while (++i < n && ret == 0)
	{
		fd = -1;
		ret = hdoc_stage(delims[i], 0, &fd);
		if (i == n - 1 && ret == 0 && fd != -1)
			hdoc_swap_fd(cmd, fd);
		else if (fd != -1)
			close(fd);
	}
	return (ret);
}
