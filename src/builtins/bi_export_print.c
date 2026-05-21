/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_pairs(char **dup, int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strncmp(dup[j], dup[j + 1],
					ft_strlen(dup[j]) + 1) > 0)
			{
				tmp = dup[j];
				dup[j] = dup[j + 1];
				dup[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_value(char *pair, char *eq)
{
	*eq = '\0';
	ft_putstr_fd(pair, STDOUT_FILENO);
	if (eq[1] == MSH_EXPORT_FLAG[0])
	{
		*eq = '=';
		ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putstr_fd(eq + 1, STDOUT_FILENO);
	ft_putstr_fd("\"", STDOUT_FILENO);
	*eq = '=';
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	bi_print_one(char *pair)
{
	char	*eq;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	eq = ft_strchr(pair, '=');
	if (eq)
		print_value(pair, eq);
	else
		ft_putendl_fd(pair, STDOUT_FILENO);
}

void	bi_print_exports(void)
{
	char	**env;
	char	**copy;
	int		n;
	int		i;

	env = env_array();
	n = env_count();
	if (n == 0)
		return ;
	copy = malloc(sizeof(char *) * n);
	if (!copy)
		return ;
	i = 0;
	while (i < n)
	{
		copy[i] = env[i];
		i++;
	}
	sort_pairs(copy, n);
	i = 0;
	while (i < n)
		bi_print_one(copy[i++]);
	free(copy);
}
