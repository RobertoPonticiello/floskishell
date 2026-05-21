/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	looks_numeric(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	bi_exit(char **argv)
{
	int	code;

	code = *status_ref();
	if (argv[1] && argv[2])
	{
		ft_fprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	if (argv[1])
	{
		if (!looks_numeric(argv[1]))
		{
			ft_fprintf(2,
				"minishell: exit: %s: numeric argument required\n", argv[1]);
			exit(2);
		}
		code = ft_atoi(argv[1]) & 0xFF;
	}
	exit(code);
}
