/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tok_free(t_tok *list)
{
	t_tok	*cur;
	t_tok	*nxt;

	cur = list;
	while (cur)
	{
		nxt = cur->nxt;
		if (cur->raw)
			free(cur->raw);
		free(cur);
		cur = nxt;
	}
}

void	cmd_close_fds(t_cmd *cmd)
{
	if (cmd->fd_in >= 0)
	{
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out >= 0)
	{
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}

static void	argv_free(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	cmd_free(t_cmd *list)
{
	t_cmd	*cur;
	t_cmd	*nxt;

	cur = list;
	while (cur)
	{
		nxt = cur->nxt;
		cmd_close_fds(cur);
		argv_free(cur->argv);
		if (cur->bin)
			free(cur->bin);
		free(cur);
		cur = nxt;
	}
}
