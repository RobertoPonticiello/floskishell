/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_hdoc_stage.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hdoc_stage(char *delim, int quoted, int *out_fd)
{
	char	*name;
	int		fd;

	name = hdoc_tmp_name();
	if (!name)
		return (-1);
	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(name);
		return (-1);
	}
	if (hdoc_read(delim, fd, quoted) == -1)
	{
		close(fd);
		unlink(name);
		free(name);
		return (-1);
	}
	close(fd);
	*out_fd = open(name, O_RDONLY);
	unlink(name);
	free(name);
	return (0);
}

void	hdoc_swap_fd(t_cmd *cmd, int fd)
{
	if (cmd->fd_in >= 0)
	{
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	cmd->fd_in = fd;
}

int	hdoc_collect(t_tok *start, char **delims, int cap)
{
	t_tok	*cur;
	int		i;

	cur = start;
	i = 0;
	while (cur && cur->kind != TK_PIPE && i < cap)
	{
		if (cur->kind == TK_HDOC && cur->nxt)
		{
			delims[i] = cur->nxt->raw;
			i++;
		}
		cur = cur->nxt;
	}
	return (i);
}
