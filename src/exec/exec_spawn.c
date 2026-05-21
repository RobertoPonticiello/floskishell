/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_spawn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	spawn_all(t_cmd *list, int **pipes, pid_t *pids, int n)
{
	t_cmd	*cur;
	int		i;

	cur = list;
	i = 0;
	while (i < n && cur)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return ;
		if (pids[i] == 0)
		{
			signals_child();
			child_pipes_setup(cur, pipes, n, i);
			child_dispatch(cur);
			exit(1);
		}
		i++;
		cur = cur->nxt;
	}
}

static void	track_signal(int status, int *sig_seen)
{
	if (!WIFSIGNALED(status))
		return ;
	if (WTERMSIG(status) == SIGINT && *sig_seen != SIGQUIT)
		*sig_seen = SIGINT;
	else if (WTERMSIG(status) == SIGQUIT)
		*sig_seen = SIGQUIT;
}

void	wait_all(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	sig_seen;

	i = 0;
	sig_seen = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			*status_ref() = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*status_ref() = 128 + WTERMSIG(status);
		track_signal(status, &sig_seen);
		i++;
	}
	if (sig_seen == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (sig_seen == SIGQUIT)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}
