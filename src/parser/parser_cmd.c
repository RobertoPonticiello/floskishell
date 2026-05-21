/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_name(const char *name)
{
	t_builtin	*table;
	int			i;

	if (!name)
		return (0);
	table = bi_table();
	i = 0;
	while (table[i].id)
	{
		if (ft_strncmp(name, table[i].id, ft_strlen(table[i].id) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

t_cmd	*cmd_new(void)
{
	t_cmd	*node;

	node = ft_calloc(1, sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->fd_in = -1;
	node->fd_out = -1;
	return (node);
}

static int	copy_argv(t_cmd *cmd, int n, char **argv)
{
	int	i;

	cmd->argv = malloc(sizeof(char *) * (n + 1));
	if (!cmd->argv)
		return (0);
	i = 0;
	while (i < n)
	{
		cmd->argv[i] = argv[i];
		i++;
	}
	cmd->argv[n] = NULL;
	return (1);
}

t_cmd	*cmd_finalize(t_cmd *cmd, int n, char **argv)
{
	if (!copy_argv(cmd, n, argv))
		return (NULL);
	if (n == 0 && !cmd->redir_err)
	{
		free(cmd->argv);
		free(cmd);
		return (NULL);
	}
	if (cmd->argv[0])
		cmd->is_builtin = is_builtin_name(cmd->argv[0]);
	return (cmd);
}

void	cmd_push(t_cmd **head, t_cmd **tail, t_cmd *node)
{
	if (!*head)
		*head = node;
	else
		(*tail)->nxt = node;
	*tail = node;
}
