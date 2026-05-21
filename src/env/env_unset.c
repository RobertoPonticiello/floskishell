/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	drop_entry(int idx, int n)
{
	char	**newer;
	char	**env;
	int		i;
	int		j;

	env = env_array();
	newer = malloc(sizeof(char *) * n);
	if (!newer)
		return ;
	i = 0;
	j = 0;
	while (i < n)
	{
		if (i != idx)
			newer[j++] = env[i];
		else
			free(env[i]);
		i++;
	}
	newer[j] = NULL;
	free(*env_ref());
	*env_ref() = newer;
}

int	env_unset(const char *name)
{
	char	**env;
	int		idx;
	int		n;

	if (!name || ft_strchr(name, '='))
		return (-1);
	idx = env_find(name);
	if (idx < 0)
		return (0);
	n = env_count();
	if (n <= 1)
	{
		env = env_array();
		if (env[0])
			free(env[0]);
		env[0] = NULL;
		return (0);
	}
	drop_entry(idx, n);
	return (0);
}

int	ident_ok(const char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
