/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*pick_target(char **argv)
{
	if (!argv[1] || ft_strncmp(argv[1], "~", 2) == 0)
		return (env_get("HOME"));
	if (ft_strncmp(argv[1], "-", 2) == 0)
		return (env_get("OLDPWD"));
	return (argv[1]);
}

static void	refresh_pwd(void)
{
	char	cwd[MSH_PATHMAX];
	char	*old_pwd;

	old_pwd = env_get("PWD");
	if (old_pwd)
		env_set("OLDPWD", old_pwd, 1);
	if (getcwd(cwd, MSH_PATHMAX))
		env_set("PWD", cwd, 1);
}

int	bi_cd(char **argv)
{
	char	*target;

	if (argv[1] && argv[2])
	{
		ft_fprintf(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	target = pick_target(argv);
	if (!target)
	{
		ft_fprintf(2, "minishell: cd: path not set\n");
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	refresh_pwd();
	return (0);
}
