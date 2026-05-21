/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	drop_fd(int *fd)
{
	if (*fd > 0)
	{
		if (close(*fd) == -1)
			perror("minishell: close");
		*fd = -1;
	}
}

static void	complain(const char *name)
{
	if (errno == ENOENT)
	{
		if (access(name, F_OK) == -1)
		{
			ft_fprintf(2, "minishell: %s: No such file or directory\n", name);
			return ;
		}
		ft_fprintf(2, "minishell: %s: Permission denied\n", name);
	}
	else
		perror("minishell");
	*status_ref() = 1;
}

static int	do_input(t_cmd *cmd, t_tok *tk)
{
	int	fd;

	if (tk->kind == TK_HDOC)
	{
		if (hdoc_count(tk) > 1)
			return (hdoc_many(cmd, tk));
		return (hdoc_one(cmd, tk));
	}
	drop_fd(&cmd->fd_in);
	fd = open(tk->nxt->raw, O_RDONLY);
	if (fd == -1)
	{
		complain(tk->nxt->raw);
		return (-1);
	}
	cmd->fd_in = fd;
	return (0);
}

static int	do_output(t_cmd *cmd, t_tok *tk)
{
	int	flags;
	int	fd;

	drop_fd(&cmd->fd_out);
	flags = O_WRONLY | O_CREAT;
	if (tk->kind == TK_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(tk->nxt->raw, flags, 0644);
	if (fd == -1)
	{
		complain(tk->nxt->raw);
		return (-1);
	}
	cmd->fd_out = fd;
	return (0);
}

int	apply_redir(t_cmd *cmd, t_tok *tk)
{
	if ((tk->kind == TK_IN || tk->kind == TK_HDOC) && tk->nxt)
		return (do_input(cmd, tk));
	if ((tk->kind == TK_OUT || tk->kind == TK_APPEND) && tk->nxt)
		return (do_output(cmd, tk));
	return (0);
}
