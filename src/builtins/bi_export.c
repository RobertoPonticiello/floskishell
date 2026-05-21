/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	bad_ident(const char *arg)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
}

static int	with_value(char *arg, char *eq)
{
	*eq = '\0';
	if (ident_ok(arg))
	{
		env_set(arg, eq + 1, 1);
		*eq = '=';
		return (0);
	}
	bad_ident(arg);
	*eq = '=';
	return (1);
}

static int	no_value(char *arg)
{
	if (ident_ok(arg))
	{
		if (!env_get(arg))
			env_set(arg, MSH_EXPORT_FLAG, 1);
		return (0);
	}
	bad_ident(arg);
	return (1);
}

int	bi_export(char **argv)
{
	char	*eq;
	int		status;
	int		i;

	if (!argv[1])
	{
		bi_print_exports();
		return (0);
	}
	status = 0;
	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
			status |= with_value(argv[i], eq);
		else
			status |= no_value(argv[i]);
		i++;
	}
	return (status);
}
