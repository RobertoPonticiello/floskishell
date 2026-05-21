/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_target(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
	{
		ft_fprintf(2, "minishell: %s: No such file or directory\n", path);
		return (127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_fprintf(2, "minishell: %s: Is a directory\n", path);
		return (126);
	}
	if (!(st.st_mode & S_IXUSR))
	{
		ft_fprintf(2, "minishell: %s: Permission denied\n", path);
		return (126);
	}
	return (0);
}

static void	exec_with_path(t_cmd *cmd)
{
	int	rc;

	rc = check_target(cmd->argv[0]);
	if (rc != 0)
		exit(rc);
	execve(cmd->argv[0], cmd->argv, env_array());
	perror("execve");
	exit(126);
}

static void	exec_via_path_var(t_cmd *cmd)
{
	if (!cmd->bin && cmd->argv && cmd->argv[0])
		cmd->bin = resolve_bin(cmd->argv[0]);
	if (cmd->bin)
	{
		execve(cmd->bin, cmd->argv, env_array());
		perror("execve");
		exit(126);
	}
	ft_fprintf(2, "minishell: %s: command not found\n", cmd->argv[0]);
	exit(127);
}

void	child_dispatch(t_cmd *cmd)
{
	apply_cmd_fds(cmd);
	if (cmd->redir_err)
		exit(1);
	if (cmd->is_builtin)
	{
		run_builtin(cmd);
		exit(*status_ref());
	}
	if (!cmd->argv || !cmd->argv[0] || ft_strlen(cmd->argv[0]) == 0)
		exit(0);
	if (ft_strchr(cmd->argv[0], '/'))
		exec_with_path(cmd);
	else
		exec_via_path_var(cmd);
}
