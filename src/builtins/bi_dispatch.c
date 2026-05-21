/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_dispatch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	stash_fds(t_cmd *cmd, int *si, int *so)
{
	if (cmd->fd_in >= 0)
	{
		*si = dup(STDIN_FILENO);
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out >= 0)
	{
		*so = dup(STDOUT_FILENO);
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}

static void	restore_fds(int si, int so)
{
	if (si >= 0)
	{
		dup2(si, STDIN_FILENO);
		close(si);
	}
	if (so >= 0)
	{
		dup2(so, STDOUT_FILENO);
		close(so);
	}
}

static int	call_named(const char *id, char **argv)
{
	t_builtin	*table;
	int			i;

	table = bi_table();
	i = 0;
	while (table[i].id)
	{
		if (ft_strncmp(id, table[i].id, ft_strlen(table[i].id) + 1) == 0)
			return (table[i].fn(argv));
		i++;
	}
	return (1);
}

void	run_builtin(t_cmd *cmd)
{
	int	saved_in;
	int	saved_out;

	saved_in = -1;
	saved_out = -1;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	stash_fds(cmd, &saved_in, &saved_out);
	*status_ref() = call_named(cmd->argv[0], cmd->argv);
	restore_fds(saved_in, saved_out);
}
