/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_printable_pair(char *pair)
{
	char	*eq;

	eq = ft_strchr(pair, '=');
	if (eq && eq[1] != MSH_EXPORT_FLAG[0])
		return (1);
	return (0);
}

static int	is_term_hidden(char *pair, int tty)
{
	if (tty)
		return (0);
	if (ft_strncmp(pair, "COLUMNS=", 8) == 0
		|| ft_strncmp(pair, "LINES=", 6) == 0)
		return (1);
	return (0);
}

int	bi_env(char **argv)
{
	char	**env;
	int		tty;
	int		i;

	(void)argv;
	env = env_array();
	tty = isatty(STDOUT_FILENO);
	i = 0;
	while (env && env[i])
	{
		if (is_printable_pair(env[i]) && !is_term_hidden(env[i], tty))
			ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
