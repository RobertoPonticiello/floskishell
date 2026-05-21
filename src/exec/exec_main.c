/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_count(t_cmd *list)
{
	int		n;
	t_cmd	*c;

	n = 0;
	c = list;
	while (c)
	{
		n++;
		c = c->nxt;
	}
	return (n);
}

static int	run_solo_fork(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	signals_silence_parent();
	pid = fork();
	if (pid == 0)
	{
		signals_child();
		child_dispatch(cmd);
	}
	if (pid < 0)
	{
		signals_init();
		return (1);
	}
	waitpid(pid, &status, 0);
	signals_init();
	return (resolve_status(status));
}

static int	run_solo(t_cmd *cmd)
{
	if (cmd->redir_err)
		return (1);
	if (cmd->is_builtin)
	{
		run_builtin(cmd);
		return (*status_ref());
	}
	if (!cmd->argv || !cmd->argv[0] || ft_strlen(cmd->argv[0]) == 0)
		return (0);
	return (run_solo_fork(cmd));
}

int	run_pipeline(t_cmd *list)
{
	int	n;

	if (!list)
		return (0);
	n = cmd_count(list);
	if (n == 1)
		return (run_solo(list));
	return (run_multi(list, n));
}
